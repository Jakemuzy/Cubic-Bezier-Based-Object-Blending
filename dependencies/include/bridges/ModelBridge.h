#ifndef _MODEL_BRIDGE_H__
#define _MODEL_BRIDGE_H__

#include <vector>

//  my stuff
#include "shapes/Model.h"
#include "MeshBridge.h"
#include "OctreeBridge.h"

//  Need to make this per instance

class ModelBridge
{
private:
public:
    std::vector<MeshBridge> meshBridges;
    std::vector<std::unique_ptr<OctreeBridge>> octreeBridges;

    void AttachModel(Model* _model);
    void Draw(IShader& shader);
    void DrawOctree(IShader& shader);
    void DrawOctreeLeafs(IShader &shader);
};


#endif