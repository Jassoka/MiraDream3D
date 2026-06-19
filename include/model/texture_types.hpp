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
private:
     PixelBuffer mPixels;
public:
    /*
    explicit Texture(QImage &img) : mPixels{} {
        //TODO: supprimer, model n'a pas besoin de fonctions de qt,
        // c'est controller qui doit se charger de la traduction en char* ou autre structure convenable
        assert(img.width() == TEXTURE_SIZE && img.height() == TEXTURE_SIZE); // Texture de taille fixe
        img = img.convertToFormat(QImage::Format_RGBA8888);
        img = img.mirrored();
        std::copy_n(img.constBits(), mPixels.size(), mPixels.begin());
    }
    */

    //TODO: on pourra peut etre surcharger l'opérateur []
    /**
     * @return Pixel (RGBA) Pointer usable by OpenGL
     */
    uint8_t* data() {
        return mPixels.data();
    }
};

#endif //MIRADREAM3D_TEXTURE_TYPES_H
