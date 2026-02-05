#pragma once
#include <cstdint>

namespace Fall {

    enum class MatterPhase : uint32_t {
        Solid,   
        Liquid,  
        Gas,     
        Plasma  
    };

    // Aligned to 16-byte boundaries for GPU SSBO performance
    struct MaterialDescriptor {
        // --- Physical State ---
        float Density;             // kg/m^3
        float Friction;            // Movement resistance
        float MeltingPoint;        // Temp where Solid -> Liquid
        float BoilingPoint;        // Temp where Liquid -> Gas

        // --- Simulation Rules ---
        float ThermalConductivity;
        uint32_t DefaultPhase;     // MatterPhase enum cast to uint
        float Viscosity;           // For Liquids
        float GravityScale;        // Buoyancy/Weight factor

        // --- Visual Intent ---
        float BaseColor[4];        // RGBA
        float Roughness;
        float Emissive;
        uint32_t VisualType;       // 0: Opaque, 1: Translucent, 2: Additive
        uint32_t TextureID;        // Index into Global Array

        float _padding[2];         // Pad to 80 bytes (multiple of 16)
    };
}