//
// Created by jassoka on 6/19/26.
//

#ifndef MIRADREAM3D_TEXTUREMANAGER_H
#define MIRADREAM3D_TEXTUREMANAGER_H
#include <GL/gl.h>
#include <list>
#include <array>
#include <QtOpenGL>

class Scene;
static constexpr uint32_t MAX_GPU_TEXTURE_LOAD = 10;

class TextureManager
{
public:
    static void initialize(QOpenGLFunctions *glFuncs);
    /**
     * @brief Loads and returns the texture associated with sceneTextureID within the scene
     */
    static GLuint loadSceneTexture(uint32_t sceneTextureID, const Scene *scene);
private:
    inline static QOpenGLFunctions *mGlFuncs = nullptr;
    static std::list<uint32_t> mShaderQueue;
    static std::array<std::list<uint32_t>::iterator, MAX_GPU_TEXTURE_LOAD> mQueueIterators;
    inline static uint32_t mNbLoadedTextures = 0;
    static std::array<GLuint, MAX_GPU_TEXTURE_LOAD> mTextureArrayID;
};

#endif //MIRADREAM3D_TEXTUREMANAGER_H
