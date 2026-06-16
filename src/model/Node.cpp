#include "model/Node.h"

Node::~Node() = default;

size_t Node::getChildrenSize() const {
    return 0;
}

bool Node::isLeaf() {
    return true;
}