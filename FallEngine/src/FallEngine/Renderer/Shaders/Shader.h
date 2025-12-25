#pragma once

#include "Core/Base.h"

#include <SDL3/SDL_gpu.h>

#include <unordered_map>
#include <string>
#include <type_traits>
#include "Memory/MappedFile.h"

namespace FallEngine {
    
    struct ShaderBlob {
        const uint8_t* code = nullptr;
        size_t size = 0;
        SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
        const char* entrypoint = nullptr;

        MappedFile backingFile;

        ShaderBlob() = default;

        ShaderBlob(const ShaderBlob&) = delete;
        ShaderBlob& operator=(const ShaderBlob&) = delete;

        ShaderBlob(ShaderBlob&&) noexcept = default;
        ShaderBlob& operator=(ShaderBlob&&) noexcept = default;
    };

    class Shader {
    public:
        explicit Shader(SDL_GPUDevice* device)
            : m_Device(device) {
        }

        virtual ~Shader() = default;
        virtual void Release() = 0;

    protected:
        SDL_GPUDevice* m_Device = nullptr;
    };

	class ShaderLibrary {
	public:
        template<typename T>
        Ref<T> Add(const std::string& name, const Ref<T>& shader) {
            static_assert(std::is_base_of_v<Shader, T>, "T must derive from Shader");
            FALL_CORE_ASSERT(!Exists(name), "Shader already exists!");

            m_Shaders[name] = shader;
            return shader;
        }

        template<typename T>
        Ref<T> Get(const std::string& name) {
            static_assert(std::is_base_of_v<Shader, T>, "T must derive from Shader");
            FALL_CORE_ASSERT(Exists(name), "Shader not found!");

            return std::static_pointer_cast<T>(m_Shaders.at(name));
        }

        bool Exists(const std::string& name) const {
            return m_Shaders.find(name) != m_Shaders.end();
        }

        void Remove(const std::string& name) {
            FALL_CORE_ASSERT(Exists(name), "Shader not found!");
            m_Shaders.erase(name);
        }

        void Clear() {
            m_Shaders.clear();
        }

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}