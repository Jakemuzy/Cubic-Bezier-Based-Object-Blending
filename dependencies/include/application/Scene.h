#ifndef _SCENE_H__
#define _SCENE_H__

#include <vector>
#include <map>

#include "Model.h"
#include "ModelBridge.h"

/*
struct ModelSceneData
{
    glm::mat4 model = glm::mat4(1.0f);
    treestumpModel = glm::rotate(treestumpModel, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    treestumpModel = glm::translate(treestumpModel, glm::vec3(-5, 0, 3.5));

    glm::mat4 projection = glm::perspective((float)glm::radians(60.0f), (float)1920.0f / (float)1080, 0.1f, 100.0f);
}
    */

class Scene
{
private:
    GraphicsRenderer& renderer;

    std::unordered_map<std::string, Model*> models;
    std::vector<ModelBridge> modelBridges;
public:
    Scene(ServiceProvider& sp) : renderer(sp.GetService<GraphicsRenderer>())
    {
        models.emplace("backpack", new Model("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack", "\\backpack.obj"));
        ModelBridge mb;
        mb.AttachModel(*models["backpack"]);
        modelBridges.push_back(mb);
    }

    ~Scene()
    {
        for (auto &model : models)
        {
            delete(model.second);
        }
    }

    void Update()
    {
        for (auto& model : models)
        {
            //model.Update();
        }
    }

    void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& proj)
    {
        //  Ideally have the model view and projection per object
        renderer.UseShader("BlockShader");
        renderer.GetShader("BlockShader").SetMat4("model", model);
        renderer.GetShader("BlockShader").SetMat4("view", view);
        renderer.GetShader("BlockShader").SetMat4("projection", proj);

        for (auto& modelBridge: modelBridges)
        {
            modelBridge.Draw(renderer.GetShader("BlockShader"));
        }
    }
};



#endif