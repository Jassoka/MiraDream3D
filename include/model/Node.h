//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_NODE_H
#define MIRADREAM3D_NODE_H
#include <vector>
#include <string>

#include "glm/fwd.hpp"


/**
 * @brief Represents a node in the object hierarchy tree
 */


class Node
{
public:
    Node(std::string name=""):mName(name){};
    virtual ~Node();
    virtual size_t getChildrenSize() const;


    virtual bool isMesh(){return false;}
    virtual bool isLeaf();

protected:
    std::string mName;
};

class LeafNode: public Node {
public:
    LeafNode(std::string name="") : Node(name) {};
    bool isLeaf() override {return true;};
    size_t getChildrenSize() const override {return 0;}
};

class HierarchyNode : public Node {

public:
    HierarchyNode(std::string &name):Node(name){};
     ~HierarchyNode() {
         for (const Node* n : mChildren)
             delete n;
         mChildren.clear();
     };
    bool isLeaf() override {return false;};

    const Node* getChild(const glm::uint32_t n) const {
        assert(n<getChildrenSize());
        return mChildren[n];
    };
    void addChild(Node* nodePtr){mChildren.push_back(nodePtr);}

    Node* popLastChild() {
        if (mChildren.empty()) return nullptr;
        Node* n = mChildren.back();
        mChildren.pop_back();
        return n;
    }
    void removeLastChild() {
        Node* n = popLastChild();
        if (n) delete n;
    }
    size_t getChildrenSize() const override {return mChildren.size();}

private:
    std::vector<Node*> mChildren;
};

class MeshNode : public LeafNode {
public:
    MeshNode(std::string name,uint32_t meshId):mMesh(meshId),LeafNode(name){};
    uint32_t getMesh() const {return mMesh;}
    void setMesh(const uint32_t mesh){mMesh=mesh;}
    bool isMesh() override {return true;}
private:
    uint32_t mMesh;

};



#endif //MIRADREAM3D_NODE_H
