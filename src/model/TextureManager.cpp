//
// Created by jassoka on 6/19/26.
//
#include "model/TextureManager.h"

#include <iostream>

#include "model/Scene.h"


std::list<uint32_t> TextureManager::mTextureCache{};

std::unordered_map<uint32_t, std::list<uint32_t>::iterator> TextureManager::mCacheIteratorsMap{};

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
#ifdef DEBUG
        std::cout << "texture est déjà chargée ";
#endif
        const auto it = mapIt->second;
        moveToFront(it);
        return mOpenGLSlotIDs[*it]; // valeur de l'itérateur
    }
    if (mUsedCache < MAX_GPU_TEXTURE_LOAD) // Il y a encore de la place
    {

#ifdef DEBUG
        std::cout << "texture jamais chargée ";
#endif
        GLtextureID = mOpenGLSlotIDs[mUsedCache];
        pushToFront(sceneTextureID);
    }
    else
    {
        //TODO: cas ou la stack est remplie
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

void TextureManager::pushToFront(const uint32_t sceneTextureID)
{
    mTextureCache.push_front(sceneTextureID);
    mCacheIteratorsMap[sceneTextureID] = mTextureCache.begin();
    mUsedCache++;
}

void TextureManager::popBack()
{
    const uint32_t id = mTextureCache.back();
    mTextureCache.pop_back();
    mCacheIteratorsMap.erase(id);
    mUsedCache--;
}

void TextureManager::moveToFront(const std::list<uint32_t>::iterator listIt)
{
    mTextureCache.splice(mTextureCache.begin(), mTextureCache,listIt);
}