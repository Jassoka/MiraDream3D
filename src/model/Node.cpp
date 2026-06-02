#include "model/Node.h"

void Node::addChild(Node* nodePtr) {
    this->mChildren.push_back(nodePtr);
}

size_t Node::getChildrenSize() const{
    return(this->mChildren.size());
}

Node* Node::getChild(const uint32_t n) const {
    assert(n<this->getChildrenSize());
    return(this->mChildren[n]);
}

void Node::addMesh(uint32_t meshId){
    this->mMeshes.push_back(meshId);
}

size_t Node::getMeshesSize() const {
    return(this->mMeshes.size());
}

uint32_t Node::getMesh(const uint32_t n) const{
    assert(n<this->getMeshesSize());
    return(this->mMeshes[n]);
}

