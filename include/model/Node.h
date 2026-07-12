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
    explicit Node(const std::string& name=""):
    mName(name){}

    virtual ~Node();

    /** @brief Returns number of children */
    virtual uint32_t getChildrenSize() const { return 0; };
    /** @brief Returns true if this node is a leaf node */
    bool isLeaf() const { return getChildrenSize() == 0; };
    /** @brief Returns a const pointer to  the n-th child of this node */
    virtual const Node* getChild(uint32_t n) const { return nullptr; }
    /** @brief Returns true if this node is a mesh */
    virtual bool isMesh() const { return false; }
    /** @brief Returns true if this node is a camera */
    virtual bool isCamera() const { return false; }
    /** @brief Returns true if this node is a light source */
    virtual bool isLight() const { return false; }
    /** @brief Returns true if this node is a hierarchy node */
    virtual bool isHierarchy() const { return false; }
    /** @brief Returns true if this node can have children */
    virtual bool isTerminal() const { return false; };

    /** @brief Returns this node's name */
    const std::string &getName() { return mName; }

protected:
    Node *mParentPtr = nullptr;
    std::string mName;
};

/** @brief Represents a leaf node in the object hierarchy tree */
class ItemNode: public Node {
public:
    using Node::Node;
    uint32_t getChildrenSize() const override { return 0; }
    const Node* getChild(uint32_t n) const override { return nullptr; }
    bool isTerminal() const override { return true; }
};


/**
 * @brief Represents a hierarchy node in the object hierarchy tree
 * A hierarchy node does not contain a mesh
 */
class HierarchyNode : public Node {
public:
    using Node::Node;
     ~HierarchyNode() override
     {
         for (const Node* n : mChildren)
             delete n;
         mChildren.clear();
     };

    const Node* getChild(const uint32_t n) const override {
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
    uint32_t getChildrenSize() const override {return mChildren.size();}

private:
    std::vector<Node*> mChildren;
};

/**
 * @brief Represents a mesh node in the object hierarchy tree
 * A mesh node cannot have children
 */
class MeshNode : public ItemNode {
public:
    MeshNode(const std::string& name, const uint32_t meshId):
        ItemNode(name),
        mMesh(meshId) {}
    /** @getter{\ref mMesh} */
    uint32_t getMesh() const {return mMesh;}
    /** @setter{\ref mMesh} */
    void setMesh(const uint32_t mesh){mMesh=mesh;}
    bool isMesh() const override {return true;}
private:
    /** @brief This node's mesh */
    uint32_t mMesh;
};



#endif //MIRADREAM3D_NODE_H
