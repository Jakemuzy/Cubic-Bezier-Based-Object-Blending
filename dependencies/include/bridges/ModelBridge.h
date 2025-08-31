#ifndef _MODEL_BRIDGE_H__
#define _MODEL_BRIDGE_H__

#include <vector>

//  my stuff
#include "Model.h"
#include "MeshBridge.h"


class ModelBridge
{
private:

public:
    static std::vector<Model*> models;
    static std::vector<MeshBridge> meshBridges;

    static void AttachModel(Model& model);
    void Draw(Shader &shader);
};


#endif