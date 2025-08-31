#include "Octree.h"

Node::Node(BoundingBox _bounds, int _currentDepth) : bounds(_bounds), currentDepth(_currentDepth)
{
    //  Slice each axis once, ie 3 2^3 = 8
    DetermineChildren(3);
}

//  Only supports 8 for now
void Node::DetermineChildren(int treeLevel)
{
    //  Currently only support octree (8 children)
    if (treeLevel >= 8)
        return;

    if (currentDepth >= maxTreeDepth)
        return;

    float midX = (bounds.min.x + bounds.max.x) * 0.5f;
    float midY = (bounds.min.y + bounds.max.y) * 0.5f;
    float midZ = (bounds.min.z + bounds.max.z) * 0.5f;

    // Left-bottom-back
    BoundingBox node1;
    node1.min = bounds.min;
    node1.max = {midX, midY, midZ};

    // Right-bottom-back
    BoundingBox node2;
    node2.min = {midX, bounds.min.y, bounds.min.z};
    node2.max = {bounds.max.x, midY, midZ};

    // Left-top-back
    BoundingBox node3;
    node3.min = {bounds.min.x, midY, bounds.min.z};
    node3.max = {midX, bounds.max.y, midZ};

    // Right-top-back
    BoundingBox node4;
    node4.min = {midX, midY, bounds.min.z};
    node4.max = {bounds.max.x, bounds.max.y, midZ};

    // Left-bottom-front
    BoundingBox node5;
    node5.min = {bounds.min.x, bounds.min.y, midZ};
    node5.max = {midX, midY, bounds.max.z};

    // Right-bottom-front
    BoundingBox node6;
    node6.min = {midX, bounds.min.y, midZ};
    node6.max = {bounds.max.x, midY, bounds.max.z};

    // Left-top-front
    BoundingBox node7;
    node7.min = {bounds.min.x, midY, midZ};
    node7.max = {midX, bounds.max.y, bounds.max.z};

    // Right-top-front
    BoundingBox node8;
    node8.min = {midX, midY, midZ};
    node8.max = bounds.max;

    children.assign({
        new Node(node1, currentDepth + 1), 
        new Node(node2, currentDepth + 1), 
        new Node(node3, currentDepth + 1), 
        new Node(node4, currentDepth + 1), 
        new Node(node5, currentDepth + 1), 
        new Node(node6, currentDepth + 1), 
        new Node(node7, currentDepth + 1), 
        new Node(node8, currentDepth + 1)
    });
}

Octree::Octree(std::vector<glm::vec3> _modelVertices) : modelVertices(_modelVertices)
{
    //  Generate Bounding Box
    float xMin = FLT_MAX, xMax = -FLT_MAX;
    float yMin = FLT_MAX, yMax = -FLT_MAX;
    float zMin = FLT_MAX, zMax = -FLT_MAX;

    for (auto &pos : modelVertices)
{
    if (pos.x < xMin) xMin = pos.x;
    if (pos.x > xMax) xMax = pos.x;

    if (pos.y < yMin) yMin = pos.y;
    if (pos.y > yMax) yMax = pos.y;

    if (pos.z < zMin) zMin = pos.z;
    if (pos.z > zMax) zMax = pos.z;
}

    
    bounds.min = glm::vec3(xMin, yMin, zMin);
    bounds.max = glm::vec3(xMax, yMax, zMax);

    //  Divide the space into 3 slices, (2^3 = 8 children)
    DetermineChildren(3);
}

/*
void CollectNodes(Node *node, std::vector<Cube> &octreeRender)
{
    if (!node)
        return;

    // push this node's bounds
    octreeRender.emplace_back(node->bounds, node->currentDepth);
    std::cout << node->currentDepth << "\n";

    // recurse through children
    for (auto *child : node->GetChildren())
    {
        CollectNodes(child, octreeRender);
    }
}
    */
