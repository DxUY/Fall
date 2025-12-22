#pragma once

#include "Core/Base.h"

#include <SDL3/SDL_gpu.h>

#include <unordered_map>
#include <string>
#include <type_traits>

namespace FallEngine {
    
    struct ShaderBlob {
        const uint8_t* code;
        size_t size;
        SDL_GPUShaderFormat format;
        const char* entrypoint;
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
        Ref<T> Add(const std::string& name, const Ref<T>& shader)
        {
            static_assert(std::is_base_of_v<Shader, T>, "T must derive from Shader");
            FALL_CORE_ASSERT(!Exists(name), "Shader already exists!");

            m_Shaders[name] = shader;
            return shader;
        }

        template<typename T>
        Ref<T> Get(const std::string& name)
        {
            static_assert(std::is_base_of_v<Shader, T>, "T must derive from Shader");
            FALL_CORE_ASSERT(Exists(name), "Shader not found!");

            return std::static_pointer_cast<T>(m_Shaders.at(name));
        }

        bool Exists(const std::string& name) const
        {
            return m_Shaders.find(name) != m_Shaders.end();
        }

        void Remove(const std::string& name)
        {
            FALL_CORE_ASSERT(Exists(name), "Shader not found!");
            m_Shaders.erase(name);
        }

        void Clear()
        {
            m_Shaders.clear();
        }

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}