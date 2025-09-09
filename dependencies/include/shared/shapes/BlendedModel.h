#ifndef _BLENDED_MODEL_H__
#define _BLENDED_MODEL_H__

#include "shapes/Model.h"
#include "Octree.h"

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
    Model& modelA, modelB;
    Model modelBlend;

    std::unique_ptr<Octree> octree = nullptr;

public:
    BlendedModel(Model& a, Model& b) : modelA(a), modelB(b) { }

};


#endif