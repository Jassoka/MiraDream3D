//
// Created by jassoka on 6/5/26.
//

#ifndef MIRADREAM3D_PRIMITIVEMESHES_HPP
#define MIRADREAM3D_PRIMITIVEMESHES_HPP
#include <cstdint>

class Mesh;
struct PrimitiveMesh;

class PrimitiveMeshes
{
public:

    static Mesh generateCube();
    /*
    static void addCubeToNode(Node &node) {//TODO faire en sorte qu'on donne un ID pour chaque forme

        node.addMesh();
    }*/

private:
    //static PrimitiveMeshes& getInstance();

    static constexpr uint32_t DEFAULT_TEXTURE = 0;
    static const PrimitiveMesh cube;
};


#endif //MIRADREAM3D_PRIMITIVEMESHES_HPP