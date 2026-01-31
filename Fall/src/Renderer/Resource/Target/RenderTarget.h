#pragma once

#include <cstdint>

namespace Fall {

    class GPUTexture; 

    enum class LoadOp : uint8_t {
        Load,
        Clear
    };

    enum class StoreOp : uint8_t {
        Store,
        Discard
    };

    struct ClearColor {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;
    };

    class RenderTarget {
    public:
        static RenderTarget Backbuffer(ClearColor clear = {}) {
            RenderTarget rt;
            rt.m_Type = Type::Backbuffer;
            rt.m_LoadOp = LoadOp::Clear;
            rt.m_StoreOp = StoreOp::Store;
            rt.m_Clear = clear;
            return rt;
        }

        static RenderTarget FromTexture(
            GPUTexture* texture,
            LoadOp load = LoadOp::Load,
            StoreOp store = StoreOp::Store,
            ClearColor clear = {}
        ) {
            RenderTarget rt;
            rt.m_Type = Type::Texture;
            rt.m_Texture = texture;
            rt.m_LoadOp = load;
            rt.m_StoreOp = store;
            rt.m_Clear = clear;
            return rt;
        }

        LoadOp GetLoadOp() const { return m_LoadOp; }
        StoreOp GetStoreOp() const { return m_StoreOp; }
        const ClearColor& GetClearColor() const { return m_Clear; }

        bool IsBackbuffer() const { return m_Type == Type::Backbuffer; }
        bool HasTexture() const { return m_Type == Type::Texture && m_Texture != nullptr; }
        GPUTexture* GetTexture() const { return m_Texture; }

    private:
        enum class Type : uint8_t {
            Backbuffer,
            Texture
        };

        Type m_Type = Type::Backbuffer;
        GPUTexture* m_Texture = nullptr;

        LoadOp m_LoadOp = LoadOp::Load;
        StoreOp m_StoreOp = StoreOp::Store;
        ClearColor m_Clear{};
    };

}