#include "ModelBridge.h"

std::vector<Model*> ModelBridge::models;
std::vector<MeshBridge> ModelBridge::meshBridges;

void ModelBridge::AttachModel(Model &model)
{
    models.push_back(&model);
    for (auto &mesh : model.GetMeshes())
    {
        meshBridges.emplace_back(mesh);
    }
}

void ModelBridge::Draw(Shader& shader)
{
    for (auto &mesh : meshBridges)
        mesh.Draw(shader);
}