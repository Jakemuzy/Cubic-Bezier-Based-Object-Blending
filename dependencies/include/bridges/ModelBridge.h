#ifndef _MODEL_BRIDGE_H__
#define _MODEL_BRIDGE_H__

#include <vector>

//  my stuff
#include "shapes/Model.h"
#include "MeshBridge.h"
#include "OctreeBridge.h"


class ModelBridge
{
private:

public:
    static std::vector<Model*> models;
    static std::vector<MeshBridge> meshBridges;
    static std::vector<OctreeBridge> octreeBridges;

    static void AttachModel(Model& model);
    void Draw(IShader& shader);
    void DrawOctree(IShader& shader);
};


#endif