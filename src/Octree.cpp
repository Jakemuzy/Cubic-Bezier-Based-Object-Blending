#include "shapes/Octree.h"

Node::Node(BoundingBox _bounds, int _currentDepth, std::vector<Triangle> _triangles) : bounds(_bounds), currentDepth(_currentDepth), triangles(std::move(_triangles))
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

    //  Determine trianlges intersecting the node
    for (auto &box : subBoxes)
    {
        std::vector<Triangle> trisInNode;
        for (auto &tri : triangles)
        {
            if (SAT(box, tri))
                trisInNode.push_back(tri);
        }

        if (!trisInNode.empty())
        {
            children.push_back(std::make_shared<Node>(box, currentDepth + 1, trisInNode));
        }
    }
}

void Node::UpdateBounds(glm::vec3 deltaMovement)
{
    bounds.min += deltaMovement;
    bounds.max += deltaMovement;

    for (auto &child : children)
    {
        child->UpdateBounds(deltaMovement);
    }
}

Octree::Octree(std::vector<Triangle> &_modelTriangles)
{
    float xMin = FLT_MAX, xMax = -FLT_MAX;
    float yMin = FLT_MAX, yMax = -FLT_MAX;
    float zMin = FLT_MAX, zMax = -FLT_MAX;

    std::function compareMinMax([&](const Vertex *v)
                                {
            xMin = std::min(xMin, v->Position.x);
            xMax = std::max(xMax, v->Position.x);
            yMin = std::min(yMin, v->Position.y);
            yMax = std::max(yMax, v->Position.y);
            zMin = std::min(zMin, v->Position.z);
            zMax = std::max(zMax, v->Position.z); });

    //  Actually copmare
    for (const auto &vert : _modelTriangles)
    {
        compareMinMax(vert.v0);
        compareMinMax(vert.v1);
        compareMinMax(vert.v2);
    }

    BoundingBox rootBounds{
        {xMin, yMin, zMin},
        {xMax, yMax, zMax}};

    // make root node
    root = std::make_shared<Node>(rootBounds, 0, _modelTriangles);
}
