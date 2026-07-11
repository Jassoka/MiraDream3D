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
