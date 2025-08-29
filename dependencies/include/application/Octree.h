#ifndef _OCTREE_H__
#define _OCTREE_H__

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
    Octree, 
    TODO: Make base class that divides space 2^n many times, instead 
    of hard coding an octree implementation

    TODO: Make a bridge that renders the octree based on the bounding boxes
*/

struct BoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};



class Node
{
private:
    Node* parent;
    std::vector<Node> children;

    int maxTreeDepth = 6;
    int currentDepth = 0;

public:
    BoundingBox bounds;

    Node() { }
    Node(BoundingBox _bounds, int _currentDepth) : bounds(_bounds), currentDepth(_currentDepth) {}
    void DetermineChildren(BoundingBox _bounds, int _childrenCount);
};

class Octree : public Node
{
private: 
    std::vector<glm::vec3> modelVertices;
public:
    Octree(std::vector<glm::vec3> _modelvertices);
};


#endif