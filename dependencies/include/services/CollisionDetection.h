#ifndef _COLLISION_DETECTION_H__
#define _COLLISION_DETECTION_H__

#include "Octree.h"

static class Collision
{
private:
    bool static BoundsIntersect(const BoundingBox& a, const BoundingBox& b)
    {
        if (!(a.min.x <= b.max.x) || !(a.max.x >= b.min.x))
            return false;
        if (!(a.min.y <= b.max.y) || !(a.max.y >= b.min.y))
            return false;
        if (!(a.min.z <= b.max.z) || !(a.max.z >= b.min.z))
            return false;
        return true;
    }
    
    bool static CompareNodes(Node* a, Node* b)
    {
        if (!BoundsIntersect(a->bounds, b->bounds))
            return false;

        if(!a->GetChildren().empty() && !b->GetChildren().empty())
        {
        }
    }

public:
    //  Make this shared_ptr, also make this take ModelData struct instead
    bool static CheckCollision(Octree* a, Octree* b)
    {
        //if (CompareNodes(a->GetRoot(), b->GetRoot()))
        if(BoundsIntersect(a->GetRoot()->bounds, b->GetRoot()->bounds))
            return true;
        return false;
    }

};

#endif