//
// Created by jassoka on 5/31/26.
//

#ifndef MIRADREAM3D_TEXTURE_TYPES_H
#define MIRADREAM3D_TEXTURE_TYPES_H

#include <cstdint>
#include <algorithm>
#include <QtGui/QImage>
constexpr uint32_t TEXTURE_SIZE = 1024;


struct Material
{
    std::string name;
    uint32_t ColorTextureID;
    /** @brief Ambiant, Diffuse and Specular coefficients
     */
    glm::vec3 Ka, Kd, Ks;
    /** @brief Transparency coefficient
     */
    float alpha;
};

/**
 * @brief Represents the RGBA pixel array for a texture
 */
using PixelBuffer = std::array<uint8_t, TEXTURE_SIZE*TEXTURE_SIZE*4>;

class Texture {
public:
    explicit Texture(const uint8_t* data)
    {
        std::memcpy(mPixels.begin(), data, TEXTURE_SIZE*TEXTURE_SIZE*4);
    }
    //TODO: on pourra peut etre surcharger l'opérateur []
    /**
     * @return Pixel (RGBA) Pointer usable by OpenGL
     */
    uint8_t* data() {
        return mPixels.data();
    }

private:
    PixelBuffer mPixels;
};

#endif //MIRADREAM3D_TEXTURE_TYPES_H
