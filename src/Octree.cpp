#include "Octree.h"

Node::Node(BoundingBox _bounds, int _currentDepth, std::vector<glm::vec3> _vertices) : bounds(_bounds), currentDepth(_currentDepth), vertices(_vertices)
{
    DetermineChildren();
}

//  Only supports 8 for now
void Node::DetermineChildren()
{
    if (currentDepth >= maxTreeDepth)
        return;

    float midX = (bounds.min.x + bounds.max.x) * 0.5f;
    float midY = (bounds.min.y + bounds.max.y) * 0.5f;
    float midZ = (bounds.min.z + bounds.max.z) * 0.5f;

    // define 8 sub-boxes
    std::vector<BoundingBox> subBoxes = {
        BoundingBox{bounds.min, {midX, midY, midZ}},                                 // LBB
        BoundingBox{{midX, bounds.min.y, bounds.min.z}, {bounds.max.x, midY, midZ}}, // RBB
        BoundingBox{{bounds.min.x, midY, bounds.min.z}, {midX, bounds.max.y, midZ}}, // LTB
        BoundingBox{{midX, midY, bounds.min.z}, {bounds.max.x, bounds.max.y, midZ}}, // RTB
        BoundingBox{{bounds.min.x, bounds.min.y, midZ}, {midX, midY, bounds.max.z}}, // LBF
        BoundingBox{{midX, bounds.min.y, midZ}, {bounds.max.x, midY, bounds.max.z}}, // RBF
        BoundingBox{{bounds.min.x, midY, midZ}, {midX, bounds.max.y, bounds.max.z}}, // LTF
        BoundingBox{{midX, midY, midZ}, bounds.max}                                  // RTF
    };

    for (auto &box : subBoxes)
    {
        // filter vertices inside box
        std::vector<glm::vec3> vertsInBox;
        for (auto &v : vertices)
        {
            if (v.x >= box.min.x && v.x <= box.max.x &&
                v.y >= box.min.y && v.y <= box.max.y &&
                v.z >= box.min.z && v.z <= box.max.z)
            {
                vertsInBox.push_back(v);
            }
        }

        if (!vertsInBox.empty())
        {
            children.push_back(std::make_unique<Node>(box, currentDepth + 1, vertsInBox));
        }
    }
}

void Node::UpdateBounds(glm::vec3 deltaMovement)
{
    bounds.min += deltaMovement;
    bounds.max += deltaMovement;

    for(auto& child : children)
    {
        child->UpdateBounds(deltaMovement);
    }
}

Octree::Octree(const std::vector<glm::vec3>& modelVertices)
{
    float xMin = FLT_MAX, xMax = -FLT_MAX;
    float yMin = FLT_MAX, yMax = -FLT_MAX;
    float zMin = FLT_MAX, zMax = -FLT_MAX;

    for (auto &pos : modelVertices)
    {
        xMin = std::min(xMin, pos.x);
        xMax = std::max(xMax, pos.x);
        yMin = std::min(yMin, pos.y);
        yMax = std::max(yMax, pos.y);
        zMin = std::min(zMin, pos.z);
        zMax = std::max(zMax, pos.z);
    }

    BoundingBox rootBounds{
        {xMin, yMin, zMin},
        {xMax, yMax, zMax}};

    // make root node
    root = std::make_unique<Node>(rootBounds, 0, modelVertices);
}
