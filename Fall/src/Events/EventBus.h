#pragma once

#include "Event.h"
#include "Core/Base.h"
#include <vector>

namespace Fall {

	class  EventQueue {
	public:
		void Submit(Scope<Event> event) {
			m_EventBuffer.push_back(std::move(event));
		}

		template<typename T>
		using EventFn = std::function<bool(T&)>;

		template<typename T>
		void Dispatch(EventFn<T> func) {
			for (auto& e : m_EventBuffer) {
				if (!e->IsHandled() && e->GetEventType() == T::GetStaticType()) {
					e->SetHandled(func(static_cast<T&>(*e)));
				}
			}
		}

		void Clear() {
			m_EventBuffer.clear();
		}

	private:
		std::vector<Scope<Event>> m_EventBuffer;
	};

	class  EventBus {
	public:
		static EventBus& Get() {
			static EventBus instance;
			return instance;
		}

		void Submit(Scope<Event> event) {
			m_MainQueue.Submit(std::move(event));
		}

		template<typename T>
		void Dispatch(EventQueue::EventFn<T> func) {
			m_MainQueue.Dispatch<T>(func);
		}

		void Clear() {
			m_MainQueue.Clear();
		}

	private:
		EventQueue m_MainQueue;
	};

}