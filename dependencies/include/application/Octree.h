#ifndef _OCTREE_H__
#define _OCTREE_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct BoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};

/*
    Octree, 
    TODO: Make base class that divides space 2^n many times, instead 
    of hard coding an octree implementation

    TODO: Make a bridge that renders the octree based on the bounding boxes
*/

class Node
{
private:
    Node* parent;
    std::vector<std::shared_ptr<Node>> children;

    int maxTreeDepth = 5;
public:
    //  Bounds in world space, verticies in local
    BoundingBox bounds;
    std::vector<glm::vec3> vertices;
    int currentDepth = 1;

    Node() = default;
    Node(BoundingBox _bounds, int _currentDepth, std::vector<glm::vec3> _vertices);

    void DetermineChildren();
    std::vector<std::shared_ptr<Node>>& GetChildren() { return children; }

    void UpdateBounds(glm::vec3 deltaMovement);
};

class Octree 
{
private:
    std::shared_ptr<Node> root;
public:
    Octree(const std::vector<glm::vec3>& _modelVertices);
    Node* GetRoot() { return root.get(); }

    //  Only works with translation for now
    void UpdateOctree(glm::vec3 deltaMovement) { root->UpdateBounds(deltaMovement); }
};



#endif