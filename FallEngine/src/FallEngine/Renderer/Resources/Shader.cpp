#include "Shader.h"

#include <fstream>

namespace FallEngine {
    Shader::Shader(const std::string& filepath)
        : m_FilePath(filepath) 
    {
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        if (lastDot != std::string::npos) {
            m_Name = filepath.substr(lastSlash, lastDot - lastSlash);
        }
        else {
            m_Name = filepath.substr(lastSlash);
        }
	}

    std::string Shader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();

            if (size > 0)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                FALL_CORE_ERROR("File '{0}' is empty or size could not be determined.", filepath);
            }
        }
        else
        {
            FALL_CORE_ERROR("Could not open file '{0}'", filepath);
            throw std::runtime_error("Failed to open shader file.");
        }

        return result;
    }

    SDL_GPUShaderStage Shader::ResolveShaderStage(const std::string& filename)
    {
        if (filename.find(".vert") != std::string::npos)
            return SDL_GPU_SHADERSTAGE_VERTEX;

        if (filename.find(".frag") != std::string::npos)
            return SDL_GPU_SHADERSTAGE_FRAGMENT;

        throw std::runtime_error("Invalid shader stage in filename: " + filename);
    }

    void Shader::CompileVulkanBinary(SDL_ShaderCross_HLSL_Info* info, size_t* size)
    {
        SDL_ShaderCross_CompileSPIRVFromHLSL(info, size);
	}

    SDL_GPUShader* Shader::LoadShader(
        SDL_GPUDevice* device,
        const char* shaderFilename,
        Uint32 samplerCount,
        Uint32 uniformBufferCount,
        Uint32 storageBufferCount,
        Uint32 storageTextureCount)
    {
        std::string source_code;
        try {
            source_code = ReadFile(m_FilePath);
        }
        catch (const std::runtime_error& e) {
            FALL_CORE_ERROR("Failed to read shader source file: {0}", m_FilePath);
            return nullptr;
        }

        SDL_GPUShaderStage stage = ResolveShaderStage(m_FilePath);

        return m_Shader;
    }

}