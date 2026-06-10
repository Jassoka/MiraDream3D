//
// Created by jassoka on 6/5/26.
//

#ifndef MIRADREAM3D_PRIMITIVEMESHES_HPP
#define MIRADREAM3D_PRIMITIVEMESHES_HPP
#include <cstdint>

class Mesh;


class PrimitiveMeshes
{
public:

    static Mesh &getCube();
    /*
    static void addCubeToNode(Node &node) {//TODO faire en sorte qu'on donne un ID pour chaque forme

        node.addMesh();
    }*/

private:
    PrimitiveMeshes();
    static PrimitiveMeshes& getInstance();
    void generateCube() const;

    static constexpr uint32_t DEFAULT_TEXTURE = 0;

    Mesh *mCube;
};


#endif //MIRADREAM3D_PRIMITIVEMESHES_HPP