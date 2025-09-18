#ifndef _COLLISION_DETECTION_H__
#define _COLLISION_DETECTION_H__

#include "Structs.h"
#include "shapes/Octree.h"

using LeafPair = std::pair<Node*, Node*>;
using LeafPairs = std::vector<LeafPair>;

class Collision
{
private:

    bool static BoundsIntersect(const BoundingBox& a, const BoundingBox& b)
    {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
               (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
               (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    void static CompareNodes(Node *a, Node *b, LeafPairs& lp)
    {
        //  Base case, they don't intersect
        if (!BoundsIntersect(a->bounds, b->bounds))
            return;

        std::vector<std::shared_ptr<Node>>& aChildren = a->GetChildren(), bChildren = b->GetChildren();

        //  Case 1, both are leaf nodes
        if (aChildren.empty() && bChildren.empty())
        {
            lp.push_back(std::pair<Node*, Node*>(a, b));
            return;
        }

        //  Case 2, both are parents
        if (!aChildren.empty() && !bChildren.empty())
        {
            for (auto &aChild : aChildren)
            {
                for (auto &bChild : bChildren)
                {
                    CompareNodes(aChild.get(), bChild.get(), lp);
                }
            }
        }

        //  Case 3, one is parent, other is leaf node
        if(!aChildren.empty() && bChildren.empty())
        {
            for (auto &aChild : aChildren)
            {
                CompareNodes(aChild.get(), b, lp);
            }
        }
        if(aChildren.empty() && !bChildren.empty())
        {
            for (auto &bChild : bChildren)
            {
                CompareNodes(a, bChild.get(), lp);
            }
        }

        
    }

public:
    //  Make this shared_ptr, also make this take ModelData struct instead
    LeafPairs static CheckCollision(Octree* a, Octree* b)
    {
        LeafPairs lp;

        CompareNodes(a->GetRoot(), b->GetRoot(), lp);
        return lp;
    }

};

#endif