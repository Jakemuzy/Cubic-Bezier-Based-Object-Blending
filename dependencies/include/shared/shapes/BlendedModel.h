#ifndef _BLENDED_MODEL_H__
#define _BLENDED_MODEL_H__

#include "EventHandler.h"

#include "shapes/Model.h"
#include "shapes/Octree.h"

using LeafPair = std::pair<Node *, Node *>;
using LeafPairs = std::vector<LeafPair>;

/*
    This is the new model created from the two blended models.
    The manner in which this is done is essentially to copy all the 
    textures and meshes from the original model, however if any of the meshes
    intersect eachother, create a new one based upon the two intersecting
    ones. From there just copy the existing Models original mesh's and add
    the new ones.
*/

class BlendedModel
{
private:
    Model* modelA = nullptr;
    Model* modelB = nullptr;

    LeafPair* CollidingNodes;
    Model modelBlend;

    std::unique_ptr<Octree> octree = nullptr;

    Event<void()> onUpdateBlend;

public:
    BlendedModel(Model& a, Model& b, LeafPair& lp) : modelA(&a), modelB(&b), CollidingNodes(&lp) { }

    //  For each node that intersects in the octree, get all the vertices in the node, 
    //  Then compare them with the intersecting nodes vertices
    void UpdateBlend();


};


#endif