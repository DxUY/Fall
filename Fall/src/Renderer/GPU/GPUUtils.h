#pragma once

namespace Fall {

    class GPUCommand;

    struct GPUUtils
    {
        static void PushDebugLabel(GPUCommand& cmd, const char* name);
        static void PopDebugLabel(GPUCommand& cmd);

        class DebugLabelScope
        {
        public:
            DebugLabelScope(GPUCommand& cmd, const char* name);
            ~DebugLabelScope();

        private:
            GPUCommand& m_Command;
        };
    };

}