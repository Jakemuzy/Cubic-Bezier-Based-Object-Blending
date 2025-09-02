#ifndef _SCENE_H__
#define _SCENE_H__

#include <vector>
#include <unordered_map>

#include "Shapes/Model.h"
#include "ModelBridge.h"

#include "IRenderer.h"

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
    OpenGLRenderer& renderer;

    std::unordered_map<std::string, std::unique_ptr<Model>> models;
    std::vector<ModelBridge> modelBridges;
public:
    Scene(IRenderer* _renderer) : renderer(*static_cast<OpenGLRenderer*>(_renderer))
    {
        models["backpack"] = std::make_unique<Model>(
            "C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack",
            "\\backpack.obj"
        );

        ModelBridge mb;
        mb.AttachModel(*models["backpack"]);
        modelBridges.push_back(mb);
    }

    // Delete copy/move because of reference member
    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    void Update()
    {
        for (auto& model : models)
        {
            //model.Update();
        }
    }

    void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& proj)
    {        

        std::cout << "draw begin\n";
        for (auto& modelBridge: modelBridges)
        {
            //  Ideally have the model view and projection per object
            renderer.UseShader("BlockShader");

            renderer.GetShader("BlockShader")->SetMat4("model", model);
            renderer.GetShader("BlockShader")->SetMat4("view", view);
            renderer.GetShader("BlockShader")->SetMat4("projection", proj);

            modelBridge.Draw(*renderer.GetShader("BlockShader"));

            /*
            renderer.UseShader("OctreeShader");
            renderer.GetShader("OctreeShader").SetMat4("model", model);
            renderer.GetShader("OctreeShader").SetMat4("view", view);
            renderer.GetShader("OctreeShader").SetMat4("projection", proj);
            modelBridge.DrawOctree(renderer.GetShader("OctreeShader"));
            */
        }

    }
};



#endif