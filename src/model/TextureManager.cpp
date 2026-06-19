//
// Created by jassoka on 6/19/26.
//
#include "model/TextureManager.h"
#include "model/Scene.h"


std::list<uint32_t> TextureManager::mShaderQueue{};

std::array<
    std::list<uint32_t>::iterator,
    MAX_GPU_TEXTURE_LOAD
> TextureManager::mQueueIterators{};

std::array<
    GLuint,
    MAX_GPU_TEXTURE_LOAD
> TextureManager::mTextureArrayID{};

GLuint TextureManager::loadSceneTexture(const uint32_t sceneTextureID, const Scene *scene)
{
    GLuint GLtextureID;
    for (const auto it : mShaderQueue)
    {
        if (it == sceneTextureID) return it; // la texture est déjà chargée
    }
    if (mNbLoadedTextures < MAX_GPU_TEXTURE_LOAD) // Il y a encore de la place
    {
        GLtextureID = mNbLoadedTextures;
        mGlFuncs->glBindTexture(GL_TEXTURE_2D, GLtextureID);
        mGlFuncs->glTexImage2D(GL_TEXTURE_2D,
            0, GL_RGBA8, TEXTURE_SIZE, TEXTURE_SIZE,
            0, GL_RGBA, GL_UNSIGNED_BYTE, scene->getTextureData(sceneTextureID));
    }
    return GLtextureID;
}

void TextureManager::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;
    for (int i = 0; i < MAX_GPU_TEXTURE_LOAD; i++)
    {
        mGlFuncs->glGenTextures(1, &mTextureArrayID[i]);
    }
}

