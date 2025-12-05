#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

namespace FallEngine {
    class Shader {
    public:
        Shader(const std::string& filepath);
        virtual ~Shader();

        SDL_GPUShader* LoadShader(
            SDL_GPUDevice* device,
            const char* shaderFilename,
            Uint32 samplerCount,
            Uint32 uniformBufferCount,
            Uint32 storageBufferCount,
            Uint32 storageTextureCount
        );

        const std::string& GetName() const { return m_Name; }
        SDL_GPUShader* GetNativeShader() const { return m_Shader; }

    private:
        std::string ReadFile(const std::string& filepath);
        SDL_GPUShaderStage ResolveShaderStage(const std::string& filename);
		void CompileVulkanBinary(SDL_ShaderCross_HLSL_Info* info, size_t* size);
    private:
        SDL_GPUShader* m_Shader = nullptr;
        std::string m_Name;
        std::string m_FilePath; 
    };
    
    class ShaderLibrary {
    public:
        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);
        bool Exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}