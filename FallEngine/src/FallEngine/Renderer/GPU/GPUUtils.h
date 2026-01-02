#pragma once

namespace FallEngine {

	class GPUCommand;

	namespace GPUUtils {

		void PushDebugLabel(GPUCommand& cmd, const char* name);
		void PopDebugLabel(GPUCommand& cmd);

		class DebugLabelScope {
		public:
			DebugLabelScope(GPUCommand& cmd, const char* name);
			~DebugLabelScope();

		private:
			GPUCommand& m_Command;
		};

	}

}