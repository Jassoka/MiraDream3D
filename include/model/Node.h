//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_NODE_H
#define MIRADREAM3D_NODE_H
#include <vector>
#include <string>

#include "glm/fwd.hpp"


/** @brief Represents a node in the object hierarchy tree */
class Node
{
public:
    explicit Node(const std::string& name=""): mName(name){};
    virtual ~Node();

    /** @brief Returns number of children */
    virtual size_t getChildrenSize() const;

    /** @brief Returns true if this node is a mesh */
    virtual bool isMesh() { return false; }
    /** @brief Returns true if this node is a leaf node */
    bool isLeaf() const { return getChildrenSize() == 0; };

protected:
    std::string mName;
};

/** @brief Represents a leaf node in the object hierarchy tree */
class LeafNode: public Node {
public:
    explicit LeafNode(const std::string& name="") : Node(name) {};
    size_t getChildrenSize() const override {return 0;}
};


/**
 * @brief Represents a hierarchy node in the object hierarchy tree
 * A hierarchy node does not contain a mesh
 */
class HierarchyNode : public Node {
public:
    explicit HierarchyNode(const std::string &name):Node(name){};
     ~HierarchyNode() override
     {
         for (const Node* n : mChildren)
             delete n;
         mChildren.clear();
     };

    /**
     * @brief Returns n-th child of this node
     */
    const Node* getChild(const uint32_t n) const {
        assert(n<getChildrenSize());
        return mChildren[n];
    };

    /**
     * @brief Adds a child to this node
     */
    void pushChild(Node* nodePtr){mChildren.push_back(nodePtr);}

    /**
     * @brief Pops last child
     */
    Node* popLastChild() {
        if (mChildren.empty()) return nullptr;
        Node* n = mChildren.back();
        mChildren.pop_back();
        return n;
    }

    /**
     * @brief Deletes last child
     */
    void deleteLastChild() {
        if (const Node* n = popLastChild()) delete n;
    }
    size_t getChildrenSize() const override {return mChildren.size();}

private:
    std::vector<Node*> mChildren;
};

/**
 * @brief Represents a mesh node in the object hierarchy tree
 * A mesh node cannot have children
 */
class MeshNode : public LeafNode {
public:
    MeshNode(const std::string& name, const uint32_t meshId):LeafNode(name),mMesh(meshId){};
    /** @getter{\ref mMesh} */
    uint32_t getMesh() const {return mMesh;}
    /** @setter{\ref mMesh} */
    void setMesh(const uint32_t mesh){mMesh=mesh;}
    bool isMesh() override {return true;}
private:
    /** @brief This node's mesh */
    uint32_t mMesh;
};



#endif //MIRADREAM3D_NODE_H
