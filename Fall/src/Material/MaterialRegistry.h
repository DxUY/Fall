#pragma once

#include "MaterialCommon.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace Fall {

    class MaterialRegistry {
    public:
        static MaterialRegistry& Get() { static MaterialRegistry instance; return instance; }

        uint16_t Register(const std::string& name, const MaterialDescriptor& desc) {
            uint16_t id = (uint16_t)m_Materials.size();
            m_Materials.push_back(desc);
            m_NameToID[name] = id;
            m_IsDirty = true;
            return id;
        }

        const std::vector<MaterialDescriptor>& GetData() const { return m_Materials; }
        bool IsDirty() const { return m_IsDirty; }
        void MarkClean() { m_IsDirty = false; }

    private:
        MaterialRegistry() { Register("Air", MaterialDescriptor{}); }
        std::vector<MaterialDescriptor> m_Materials;
        std::unordered_map<std::string, uint16_t> m_NameToID;
        bool m_IsDirty = true;
    };

}