#pragma once

#include <string>
#include "Shader.h"

namespace FallEngine {

    class ShaderCompiler {
    public:
        static ShaderBlob LoadSPIRV(const std::string& path);
		static ShaderBlob LoadGLSL(const std::string& path);

		//Needs Compile function to convert GLSL to SPIR-V, both in runtime and precompile step
    };

}
