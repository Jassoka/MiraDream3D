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

        // ==========================================
        // DÉFINITION DES FACES (6 Quads au lieu de 12 Triangles)
        // ==========================================

        // Face Avant (+Z)
        mCube.addQuad({ 2, 14, 20, 8 });

        // Face Arrière (-Z)
        mCube.addQuad({ 17, 5, 11, 23 });

        // Face Haut (+Y)
        mCube.addQuad({ 13, 1, 4, 16 });

        // Face Bas (-Y)
        mCube.addQuad({ 19, 22, 10, 7 });

        // Face Droite (+X)
        mCube.addQuad({ 0, 6, 9, 3 });

        // Face Gauche (-X)
        mCube.addQuad({ 15, 21, 18, 12 });
        mCube.triangulate() ;
        mCube.generateEdges();
        #ifdef TEST_HALFEDGES
        //mCube.mHalfEdges.push_back((HalfEdge){0,0,0,0,0,6});
        //mCube.mHalfEdges.push_back((HalfEdge){0,0,0,0,6,9});
        //mCube.mHalfEdges.push_back((HalfEdge){0,0,0,0,9,3});
        //mCube.mHalfEdges.push_back((HalfEdge){0,0,0,0,3,0});
        #endif
    };

    static constexpr uint32_t DEFAULT_TEXTURE = 0;

    Mesh mCube;
};


#endif //MIRADREAM3D_PRIMITIVEMESHES_HPP