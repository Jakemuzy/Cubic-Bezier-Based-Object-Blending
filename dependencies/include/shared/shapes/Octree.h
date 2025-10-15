#ifndef _OCTREE_H__
#define _OCTREE_H__

#include <vector>
#include <memory>

#include "Structs.h"

#include "math/IntersectionLogic.h"
#include "EventHandler.h"

/*
    Octrees essentially divide the model into easier to 
    interact with pieces. The trouble here was having triangles
    intersect with the bounding boxes of the octree child nodes.
    The prior naive method was to simply initalize bounds based
    on the vertices of a model.
*/

class Node
{
private:
    Node* parent;
    std::vector<std::shared_ptr<Node>> children;

    int maxTreeDepth = 1;
public:
    //  Bounds in world space, verticies in local
    BoundingBox bounds;
    std::vector<Triangle> triangles;

    Event<bool> onIntersection;
    int currentDepth = 1;


    Node() = default;
    Node(BoundingBox _bounds, int _currentDepth, std::vector<Triangle> _triangles);

    void DetermineChildren();
    std::vector<std::shared_ptr<Node>>& GetChildren() { return children; }

    void UpdateBounds(glm::vec3 deltaMovement);
};

class Octree 
{
private:
    std::shared_ptr<Node> root;
public:
    Octree(std::vector<Triangle>& _modelTriangles);
    Node* GetRoot() { return root.get(); }

    //  Only works with translation for now
    void UpdateOctree(glm::vec3 deltaMovement) { root->UpdateBounds(deltaMovement); }
};



#endif