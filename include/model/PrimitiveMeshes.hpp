//
// Created by jassoka on 6/5/26.
//

#ifndef MIRADREAM3D_PRIMITIVEMESHES_HPP
#define MIRADREAM3D_PRIMITIVEMESHES_HPP


#include "model/Mesh.h"


class PrimitiveMeshes
{
public:

    static Mesh &getCube()
    {
        return getInstance().mCube;
    };
private:
    PrimitiveMeshes(): mCube(Mesh(DEFAULT_TEXTURE))
    {
        generateCube();
    }
    static PrimitiveMeshes& getInstance()
    {
        static auto instance = PrimitiveMeshes();
        return instance;
    }
    void generateCube()
    {
        constexpr float n = 0.577350f;
        mCube.addVertex({ 1.0f,  1.0f,  1.0f,  n,  n,  n, 0.0f, 0.0f});
        mCube.addVertex({ 1.0f,  1.0f, -1.0f,  n,  n, -n, 0.0f, 0.0f});
        mCube.addVertex({ 1.0f, -1.0f,  1.0f,  n, -n,  n, 0.0f, 0.0f});
        mCube.addVertex({ 1.0f, -1.0f, -1.0f,  n, -n, -n, 0.0f, 0.0f});
        mCube.addVertex({-1.0f,  1.0f,  1.0f, -n,  n,  n, 0.0f, 0.0f});
        mCube.addVertex({-1.0f,  1.0f, -1.0f, -n,  n, -n, 0.0f, 0.0f});
        mCube.addVertex({-1.0f, -1.0f,  1.0f, -n, -n,  n, 0.0f, 0.0f});
        mCube.addVertex({-1.0f, -1.0f, -1.0f, -n, -n, -n, 0.0f, 0.0f});

        // 2. Ajout des faces (Indices convertis : OBJ-1 = C++)
        // Face Avant
        mCube.addFace({0, 4, 6});
        mCube.addFace({0, 6, 2});

        // Face Arrière
        mCube.addFace({5, 1, 3});
        mCube.addFace({5, 3, 7});

        // Face Haut
        mCube.addFace({4, 0, 1});
        mCube.addFace({4, 1, 5});

        // Face Bas
        mCube.addFace({6, 7, 3});
        mCube.addFace({6, 3, 2});

        // Face Droite
        mCube.addFace({0, 2, 3});
        mCube.addFace({0, 3, 1});

        // Face Gauche
        mCube.addFace({5, 7, 6}); // 6,8,7 -> 5,7,6
        mCube.addFace({5, 6, 4}); // 6,7,5 -> 5,6,4
    };

    static constexpr uint32_t DEFAULT_TEXTURE = 0;

    Mesh mCube;
};


#endif //MIRADREAM3D_PRIMITIVEMESHES_HPP