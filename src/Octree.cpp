#include "Octree.h"

//  Only supports 8 for now
void Node::DetermineChildren(BoundingBox _bounds, int _childrenCount)
{
    // Currently only support octree (8 children)
    if (_childrenCount != 3)
    {
        throw std::runtime_error("DetermineChildren only supports 3D octree (8 children)");
    }

    float midX = (_bounds.min.x + _bounds.max.x) * 0.5f;
    float midY = (_bounds.min.y + _bounds.max.y) * 0.5f;
    float midZ = (_bounds.min.z + _bounds.max.z) * 0.5f;

    // Left-bottom-back
    BoundingBox node1;
    node1.min = _bounds.min;
    node1.max = {midX, midY, midZ};

    // Right-bottom-back
    BoundingBox node2;
    node2.min = {midX, _bounds.min.y, _bounds.min.z};
    node2.max = {_bounds.max.x, midY, midZ};

    // Left-top-back
    BoundingBox node3;
    node3.min = {_bounds.min.x, midY, _bounds.min.z};
    node3.max = {midX, _bounds.max.y, midZ};

    // Right-top-back
    BoundingBox node4;
    node4.min = {midX, midY, _bounds.min.z};
    node4.max = {_bounds.max.x, _bounds.max.y, midZ};

    // Left-bottom-front
    BoundingBox node5;
    node5.min = {_bounds.min.x, _bounds.min.y, midZ};
    node5.max = {midX, midY, _bounds.max.z};

    // Right-bottom-front
    BoundingBox node6;
    node6.min = {midX, _bounds.min.y, midZ};
    node6.max = {_bounds.max.x, midY, _bounds.max.z};

    // Left-top-front
    BoundingBox node7;
    node7.min = {_bounds.min.x, midY, midZ};
    node7.max = {midX, _bounds.max.y, _bounds.max.z};

    // Right-top-front
    BoundingBox node8;
    node8.min = {midX, midY, midZ};
    node8.max = _bounds.max;

    // Store them somewhere (assuming you have a vector<Node> children)
    children.resize(8);
    children = { node1, node2, node3, node4, node5, node6, node7, node8 };
}

Octree::Octree(Model model)
{
    float xMin = FLT_MIN, xMax = FLT_MAX, yMin = FLT_MIN, yMax = FLT_MAX, zMin = FLT_MIN, zMax = FLT_MAX;

    for (auto &mesh : model.GetMeshes())
    {
        for (auto &vertex : mesh.vertices)
        {
            glm::vec3 pos = vertex.Position;

            if (pos.x < xMin)
                xMin = pos.x;
            else if (pos.x > xMax)
                xMax = pos.x;

            if (pos.y < yMin)
                yMin = pos.y;
            else if (pos.y > yMax)
                yMax = pos.y;

            if (pos.z < zMin)
                zMin = pos.z;
            else if (pos.z > zMax)
                zMax = pos.z;
        }
    }
    
    bounds.min = glm::vec3(xMin, yMin, zMin);
    bounds.max = glm::vec3(xMax, yMax, zMax);

    //  Divide the space into 3 slices, (2^3 = 8 children)
    DetermineChildren(bounds, 3);
}