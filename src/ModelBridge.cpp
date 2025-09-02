#include "ModelBridge.h"

std::vector<Model*> ModelBridge::models;
std::vector<MeshBridge> ModelBridge::meshBridges;
std::vector<OctreeBridge> ModelBridge::octreeBridges;

void ModelBridge::AttachModel(Model &model)
{
    models.push_back(&model);
    octreeBridges.emplace_back(model.octree->bounds, 1);

    for (auto &mesh : model.GetMeshes())
    {
        meshBridges.emplace_back(mesh);
    }
}

void ModelBridge::Draw(IShader& shader)
{
    for (auto &mesh : meshBridges)
        mesh.Draw(shader);
}

void ModelBridge::DrawOctree(IShader& shader)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(auto& octree : octreeBridges)
        octree.Draw(shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}