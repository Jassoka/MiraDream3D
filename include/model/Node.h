//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_NODE_H
#define MIRADREAM3D_NODE_H
#include <vector>
#include <cstdint>

/**
 * @brief Represents a node in the object hierarchy tree
 */
class Node
{
public:
    //TODO DESTRUCTEUR

    void addChild(Node* nodePtr);
    size_t getChildrenSize() const;
    Node* getChild(uint32_t n) const;

    void addMesh(uint32_t meshId);
    size_t getMeshesSize() const;
    uint32_t getMesh(uint32_t n) const;



private:
    std::vector<uint32_t> mMeshes;
    std::vector<Node*> mChildren;
};

#endif //MIRADREAM3D_NODE_H
