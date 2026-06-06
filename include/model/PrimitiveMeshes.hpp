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
    /*
    static void addCubeToNode(Node &node) {//TODO faire en sorte qu'on donne un ID pour chaque forme

        node.addMesh();
    }*/

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

    // === Coin (+X, +Y, +Z) ===
    mCube.addVertex({ 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); //  0 → face Droite
    mCube.addVertex({ 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f}); //  1 → face Haut
    mCube.addVertex({ 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f}); //  2 → face Avant

    // === Coin (+X, +Y, -Z) ===
    mCube.addVertex({ 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); //  3 → face Droite
    mCube.addVertex({ 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f}); //  4 → face Haut
    mCube.addVertex({ 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f}); //  5 → face Arrière

    // === Coin (+X, -Y, +Z) ===
    mCube.addVertex({ 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); //  6 → face Droite
    mCube.addVertex({ 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f}); //  7 → face Bas
    mCube.addVertex({ 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f}); //  8 → face Avant

    // === Coin (+X, -Y, -Z) ===
    mCube.addVertex({ 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); //  9 → face Droite
    mCube.addVertex({ 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f}); // 10 → face Bas
    mCube.addVertex({ 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f}); // 11 → face Arrière

    // === Coin (-X, +Y, +Z) ===
    mCube.addVertex({-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); // 12 → face Gauche
    mCube.addVertex({-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f}); // 13 → face Haut
    mCube.addVertex({-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f}); // 14 → face Avant

    // === Coin (-X, +Y, -Z) ===
    mCube.addVertex({-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); // 15 → face Gauche
    mCube.addVertex({-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f}); // 16 → face Haut
    mCube.addVertex({-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f}); // 17 → face Arrière

    // === Coin (-X, -Y, +Z) ===
    mCube.addVertex({-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); // 18 → face Gauche
    mCube.addVertex({-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f}); // 19 → face Bas
    mCube.addVertex({-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f}); // 20 → face Avant

    // === Coin (-X, -Y, -Z) ===
    mCube.addVertex({-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f}); // 21 → face Gauche
    mCube.addVertex({-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f}); // 22 → face Bas
    mCube.addVertex({-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f}); // 23 → face Arrière

    // Face Avant  (+Z) : sommets →2 (indices *2), 8, 14, 20
    mCube.addFace({ 2, 14, 20});
    mCube.addFace({ 2, 20,  8});

    // Face Arrière (-Z) : sommets → 17, 5, 11, 23
    mCube.addFace({17,  5, 11});
    mCube.addFace({17, 11, 23});

    // Face Haut   (+Y) : sommets → 13, 1, 4, 16
    mCube.addFace({13,  1,  4});
    mCube.addFace({13,  4, 16});

    // Face Bas    (-Y) : sommets → 19, 22, 10, 7
    mCube.addFace({19, 22, 10});
    mCube.addFace({19, 10,  7});

    // Face Droite (+X) : sommets → 0, 6, 9, 3
    mCube.addFace({ 0,  6,  9});
    mCube.addFace({ 0,  9,  3});

    // Face Gauche (-X) : sommets → 15, 21, 18, 12
    mCube.addFace({15, 21, 18});
    mCube.addFace({15, 18, 12});
    };

    static constexpr uint32_t DEFAULT_TEXTURE = 0;

    Mesh mCube;
};


#endif //MIRADREAM3D_PRIMITIVEMESHES_HPP