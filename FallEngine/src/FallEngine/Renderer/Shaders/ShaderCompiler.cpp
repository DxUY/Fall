#include "FallEnginePCH.h"
#include "ShaderCompiler.h"
#include "Core/FileSystem.h"

namespace FallEngine {

    ShaderBlob ShaderCompiler::LoadSPIRV(const std::string& path) {
        ShaderBlob blob{};
        blob.backingFile = FileSystem::MapFileBinary(path);
        FALL_CORE_ASSERT(blob.backingFile.IsOpen(), "Failed to map shader file");

        BufferView view = blob.backingFile.View();

        blob.code = view.Data();
        blob.size = view.Size();
        blob.format = SDL_GPU_SHADERFORMAT_SPIRV;
        blob.entrypoint = "main";

        return blob;
    }

}