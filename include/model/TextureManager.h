//
// Created by jassoka on 6/19/26.
//

#ifndef MIRADREAM3D_TEXTUREMANAGER_H
#define MIRADREAM3D_TEXTUREMANAGER_H
#include <GL/gl.h>
#include <array>
#include <QtOpenGL>

class Scene;

/** @brief Max amount of textures loaded in GPU */
static constexpr uint32_t MAX_GPU_TEXTURE_LOAD = 10;


/** @brief Class for managing texture loading in GPU*/
class TextureManager
{
public:
    static void initialize(QOpenGLFunctions *glFuncs);
    /** @brief Loads and returns the texture associated with sceneTextureID within the scene */
    static GLuint loadSceneTexture(uint32_t sceneTextureID, const Scene *scene);
private:
    /**
     * @brief For a given cached texture, we have:
     * An \ref mOpenGLSlotIDs index
     * A texture ID
     */
    struct TextureStackElement
    {
        uint32_t openGLSlotID;
        uint32_t textureID;
    };
    using TextureStack = std::list<TextureStackElement>;

    /** @brief Pushes a texture to the top of \ref mTextureCache stack */
    static void pushToFront(uint32_t sceneTextureID, uint32_t availableOpenGLSlot);

    /**
     * @brief Pops bottom of \ref mTextureCache stack
     * @return Freed \ref mOpenGLSlotIDs element
     */
    [[nodiscard]] static uint32_t popBack();

    /**
     * @brief Moves a loaded texture to the front of the stack
     * @param listIt Iterator of \ref mTextureCache element
     */
    static void moveToFront(TextureStack::iterator listIt);
    inline static QOpenGLFunctions *mGlFuncs = nullptr;

    /**
     * @brief Stack of \ref TextureStackElement objects
     * Front is the most recently used texture (top of the stack)
     * Back is the least recently used texture (bottom of stack)
     */
    static TextureStack mTextureCache;
    /** @brief Map associated a texture ID to a \ref mTextureCache element iterator */
    static std::unordered_map<uint32_t, TextureStack::iterator> mCacheIteratorsMap;
    /** @brief \ref mTextureCache size */
    inline static uint32_t mUsedCache = 0;
    /** @brief Array for \ref MAX_GPU_TEXTURE_LOAD OpenGL available texture slots */
    static std::array<GLuint, MAX_GPU_TEXTURE_LOAD> mOpenGLSlotIDs; // array for storing available OpenGL slots
};

#endif //MIRADREAM3D_TEXTUREMANAGER_H
