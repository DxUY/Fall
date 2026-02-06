#pragma once
#include <cstdint>

namespace Fall {

    enum class MatterPhase : uint32_t {
        Solid, 
        Liquid, 
        Gas, 
        Plasma
    };

    struct MaterialDescriptor {
        float Density = 1.0f;
        float Friction = 0.5f;
        float MeltingPoint = 1000.0f;
        float BoilingPoint = 2000.0f;

        float ThermalConductivity = 0.1f;
        uint32_t DefaultPhase = 0;
        float Viscosity = 0.5f;
        float GravityScale = 1.0f;

        float BaseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        float Roughness = 0.5f;
        float Emissive = 0.0f;
        uint32_t VisualType = 0; 
        uint32_t TextureID = 0;
    };

    static_assert(sizeof(MaterialDescriptor) % 16 == 0, "MaterialDescriptor alignment error!");

}