#include "ModelBridge.h"

void ModelBridge::AttachModel(Model* _model)
{
    octreeBridges.push_back(std::make_unique<OctreeBridge>(_model->octree.get()));
    for (auto &mesh : (*_model).GetMeshes())
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
        octree->Draw(shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}