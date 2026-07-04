//
// Created by jassoka on 6/19/26.
//

#ifndef MIRADREAM3D_TEXTUREMANAGER_H
#define MIRADREAM3D_TEXTUREMANAGER_H
#include <GL/gl.h>
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
    static void pushToFront(uint32_t sceneTextureID);
    static void popBack();
    static void moveToFront(const std::list<uint32_t>::iterator listIt);
    inline static QOpenGLFunctions *mGlFuncs = nullptr;
    static std::list<uint32_t> mTextureCache; // stack for the indexes of mOpenGLSlotIDs
    static std::unordered_map<uint32_t, std::list<uint32_t>::iterator> mCacheIteratorsMap; // texture ID -> iterator of mTextureCache
    inline static uint32_t mUsedCache = 0; // top of stack
    static std::array<GLuint, MAX_GPU_TEXTURE_LOAD> mOpenGLSlotIDs; // array for storing available OpenGL slots
};

#endif //MIRADREAM3D_TEXTUREMANAGER_H
