#include "model/Node.h"
#include <cassert>



void Node::addChild(Node* nodePtr) {
    mChildren.push_back(nodePtr);
}

size_t Node::getChildrenSize() const{
    return mChildren.size();
}

Node* Node::getChild(const uint32_t n) const {
    assert(n<getChildrenSize());
    return mChildren[n];
}

void Node::addMesh(uint32_t meshId){
    mMeshes.push_back(meshId);
}

size_t Node::getMeshesSize() const {
    return mMeshes.size();
}

uint32_t Node::getMesh(const uint32_t n) const{
    assert(n<getMeshesSize());
    return mMeshes[n];
}

