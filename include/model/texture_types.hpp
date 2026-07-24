//
// Created by jassoka on 5/31/26.
//

#ifndef MIRADREAM3D_TEXTURE_TYPES_H
#define MIRADREAM3D_TEXTURE_TYPES_H

#include <cstdint>
#include <algorithm>
#include <QtGui/QImage>
#include <glm/vec3.hpp>
constexpr int TEXTURE_SIZE = 1024;


struct Material
{
    Material():
        ColorTextureID(DEFAULT_TEXTURE),
        Ka({0.15f, 0.15f, 0.15f}),
        Kd({0.7f, 0.7f, 0.7f}),
        Ks({0.2f, 0.2f, 0.2f}),
        alpha(1.0f),
        shininess(32.0f) {}
    uint32_t ColorTextureID;
    /** @brief Ambiant coefficient */
    glm::vec3 Ka;
    /** @brief Diffuse coefficient */
    glm::vec3 Kd;
    /** @brief Specular coefficient */
    glm::vec3 Ks;
    /** @brief Transparency coefficient */
    float alpha;
    float shininess;
};

struct MaterialRegistry
{
    /** @brief List of materials in registry */
    std::vector<Material> materials;
    /** @brief Map associating a name to each Material ID */
    std::unordered_map<std::string, uint32_t> materialNames;

    /**
     * @brief Creates a new material and returns its ID
     * @param name New material name
     * @param material Optional material copied to the new slot
     * cf. \ref Material::Material() for default material
     */
    uint32_t newMaterial(const std::string &name, const Material& material = Material())
    {
        // On check si un materiau de meme nom n'existe pas déjà
        const auto it = materialNames.find(name);
        if (it == materialNames.end())
        {
            // S'il n'y en a pas on crée un materiau
            materials.push_back(material);
            materialNames[name]=materials.size()-1;
        }
        return materialNames[name];
    }

    /**
     * @brief Returns ID to material called name
     * Returns -1 if material does not exist
     */
    int32_t getMaterialID(const std::string &name)
    {
        if (materialNames.find(name) == materialNames.end()) return -1;
        return materialNames[name];
    }
    /**
     * @brief Returns const reference to material at index id
     */
    const Material &getMaterial(const uint32_t id) const
    {
        assert(id < materials.size());
        return materials[id];
    }
    /**
     * @brief Returns reference to material at index id
     */
    Material &getMaterial(const uint32_t id)
    {
        assert(id < materials.size());
        return materials[id];
    }
};

/**
 * @brief Represents the RGBA pixel array for a texture
 */
using PixelBuffer = std::array<uint8_t, TEXTURE_SIZE*TEXTURE_SIZE*4>;

class Texture {
public:
    explicit Texture(const uint8_t* data) { std::memcpy(mPixels.data(), data, mPixels.size()); }
    /** @return Pixel (RGBA) Raw data Pointer */
    const uint8_t* data() const { return mPixels.data(); }

private:
    PixelBuffer mPixels;
};

#endif //MIRADREAM3D_TEXTURE_TYPES_H
