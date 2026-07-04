//
// Created by jassoka on 6/19/26.
//
#include "model/TextureManager.h"

#include <fcntl.h>
#include <iostream>

#include "model/Scene.h"


TextureManager::TextureStack TextureManager::mTextureCache{};

std::unordered_map<uint32_t, TextureManager::TextureStack::iterator> TextureManager::mCacheIteratorsMap{};

std::array<
    GLuint,
    MAX_GPU_TEXTURE_LOAD
> TextureManager::mOpenGLSlotIDs{};

GLuint TextureManager::loadSceneTexture(const uint32_t sceneTextureID, const Scene *scene)
{
    GLuint GLtextureID;
    const auto mapIt = mCacheIteratorsMap.find(sceneTextureID);
    if (mapIt != mCacheIteratorsMap.end()) // existe déjà
    {
        const auto it = mapIt->second;
        moveToFront(it);
        return mOpenGLSlotIDs[it->openGLSlotID]; // valeur de l'itérateur
    }
    if (mUsedCache < MAX_GPU_TEXTURE_LOAD) // Il y a encore de la place
    {
        GLtextureID = mOpenGLSlotIDs[mUsedCache];
        pushToFront(sceneTextureID, mUsedCache);
    }
    else
    {
        const uint32_t glAvailableSlot = popBack();
        pushToFront(sceneTextureID, glAvailableSlot);
        GLtextureID = mOpenGLSlotIDs[glAvailableSlot];
    }

    mGlFuncs->glBindTexture(GL_TEXTURE_2D, GLtextureID);
    mGlFuncs->glTexImage2D(GL_TEXTURE_2D,
        0, GL_RGBA8, TEXTURE_SIZE, TEXTURE_SIZE,
        0, GL_RGBA, GL_UNSIGNED_BYTE, scene->getTextureData(sceneTextureID));
    mGlFuncs->glGenerateMipmap(GL_TEXTURE_2D);
    return GLtextureID;
}

void TextureManager::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;
    for (int i = 0; i < MAX_GPU_TEXTURE_LOAD; i++)
    {
        mGlFuncs->glGenTextures(1, &mOpenGLSlotIDs[i]);
    }
}

void TextureManager::pushToFront(const uint32_t sceneTextureID, const uint32_t availableOpenGLSlot)
{
    mTextureCache.push_front(TextureStackElement{availableOpenGLSlot, sceneTextureID});
    mCacheIteratorsMap[sceneTextureID] = mTextureCache.begin();
    mUsedCache++;
}

uint32_t TextureManager::popBack()
{
    const uint32_t textureID = mTextureCache.back().textureID;
    const uint32_t openGLSlotID = mTextureCache.back().openGLSlotID;
    mTextureCache.pop_back();
    mCacheIteratorsMap.erase(textureID);
    mUsedCache--;
    return openGLSlotID;
}

void TextureManager::moveToFront(const TextureStack::iterator listIt)
{
    mTextureCache.splice(mTextureCache.begin(), mTextureCache,listIt);
}