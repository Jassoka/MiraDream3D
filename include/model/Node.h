//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_NODE_H
#define MIRADREAM3D_NODE_H
#include <vector>

#include "Mesh.h"

/**
 * @brief Represents a node in the object hierarchy tree
 */
class Node
{
private:
    Mesh *mMesh = nullptr;
    std::vector<Node*> mChildren;
};

#endif //MIRADREAM3D_NODE_H
