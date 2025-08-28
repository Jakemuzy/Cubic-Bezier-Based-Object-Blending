#ifndef _OCTREE_H__
#define _OCTREE_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

/*
    Octree, 
    TODO: Make base class that divides space 2^n many times, instead 
    of hard coding an octree implementation
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

    int spaceDivisions = 0;

public:
    BoundingBox bounds;

    Node();
    Node(BoundingBox _bounds, int _spaceDivisions) : bounds(_bounds), spaceDivisions(_spaceDivisions) {}
    void DetermineChildren(BoundingBox _bounds, int _childrenCount);
};

class Octree : public Node
{
private: 
    Model* model;
public:
    Octree(Model model);
};



#endif