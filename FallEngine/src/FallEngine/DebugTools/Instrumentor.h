#pragma once

#include "Log.h"
#include "FallEngine/Core/Base.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

namespace FallEngine {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult {
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession {
		std::string Name;
	};

	class Instrumentor {
	public:
		FALL_NON_COPYABLE(Instrumentor)

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession) {
				
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					FALL_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open()) {
				m_CurrentSession = std::make_unique<InstrumentationSession>(InstrumentationSession{ name });
				WriteHeader();
			} else {
				if (Log::GetCoreLogger()) { // Edge case: BeginSession() might be before Log::Init()
					FALL_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession() {
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result) {
			const std::string escapedName = EscapeJsonString(result.Name);

			uint64_t tid;
			{
				std::lock_guard<std::mutex> lock(m_Mutex);

				auto it = m_ThreadIdMap.find(result.ThreadID);
				if (it == m_ThreadIdMap.end()) {
					tid = m_NextThreadId++;
					m_ThreadIdMap.emplace(result.ThreadID, tid);
				}
				else {
					tid = it->second;
				}

				if (!m_CurrentSession)
					return;

				if (!m_FirstEvent)
					m_OutputStream << ",";
				else
					m_FirstEvent = false;

				m_OutputStream << "{";
				m_OutputStream << "\"cat\":\"function\",";
				m_OutputStream << "\"dur\":" << (result.ElapsedTime.count()) << ",";
				m_OutputStream << "\"name\":\"" << escapedName << "\",";
				m_OutputStream << "\"ph\":\"X\",";
				m_OutputStream << "\"pid\":0,";
				m_OutputStream << "\"tid\":" << tid << ",";
				m_OutputStream << "\"ts\":" << result.Start.count();
				m_OutputStream << "}";
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_CurrentSession(nullptr), m_NextThreadId(1), m_FirstEvent(true) {}

		~Instrumentor() {
			EndSession();
		}

		void WriteHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static std::string EscapeJsonString(const std::string& s) {
			std::string out;
			out.reserve(s.size());
			for (unsigned char uc : s) {
				char c = static_cast<char>(uc);
				switch (c) {
				case '\\': out += "\\\\"; break;
				case '"':  out += "\\\""; break;
				case '\b': out += "\\b"; break;
				case '\f': out += "\\f"; break;
				case '\n': out += "\\n"; break;
				case '\r': out += "\\r"; break;
				case '\t': out += "\\t"; break;
				default:
					if (uc < 0x20) {
						char buf[7];
						std::snprintf(buf, sizeof(buf), "\\u%04x", uc);
						out += buf;
					}
					else {
						out += c;
					}
				}
			}
			return out;
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession() {
			if (m_CurrentSession) {
				WriteFooter();
				m_OutputStream.close();
				m_CurrentSession.reset();
			}
		}
	private:
		std::mutex m_Mutex;
		std::unique_ptr<InstrumentationSession> m_CurrentSession;
		std::ofstream m_OutputStream;

		std::unordered_map<std::thread::id, uint64_t> m_ThreadIdMap;
		uint64_t m_NextThreadId;
		bool m_FirstEvent;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false) {
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer() {
			if (!m_Stopped)
				Stop();
		}

		void Stop() {
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult {
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K]) {
			ChangeResult<N> result = {};
			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N && dstIndex + 1 < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				char c = expr[srcIndex];
				result.Data[dstIndex++] = (c == '"') ? '\'' : c;
				srcIndex++;
			}
			if (dstIndex < N)
				result.Data[dstIndex] = '\0';
			else
				result.Data[N - 1] = '\0';
			return result;
		}
	}
}

#define F_PROFILE 0
#if F_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define F_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define F_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define F_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define F_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define F_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define F_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define F_FUNC_SIG __func__
#else
#define F_FUNC_SIG "F_FUNC_SIG unknown!"
#endif

#define F_PROFILE_BEGIN_SESSION(name, filepath) ::FallEngine::Instrumentor::Get().BeginSession(name, filepath)
#define F_PROFILE_END_SESSION() ::FallEngine::Instrumentor::Get().EndSession()
#define F_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::FallEngine::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::FallEngine::InstrumentationTimer timer##line(fixedName##line.Data)
#define F_PROFILE_SCOPE_LINE(name, line) F_PROFILE_SCOPE_LINE2(name, line)
#define F_PROFILE_SCOPE(name) F_PROFILE_SCOPE_LINE(name, __LINE__)
#define F_PROFILE_FUNCTION() F_PROFILE_SCOPE(F_FUNC_SIG)
#else
#define F_PROFILE_BEGIN_SESSION(name, filepath)
#define F_PROFILE_END_SESSION()
#define F_PROFILE_SCOPE(name)
#define F_PROFILE_FUNCTION()
#endif