#ifndef _SCENE_H__
#define _SCENE_H__

#include <vector>
#include <unordered_map>

#include "Shapes/Model.h"
#include "ModelBridge.h"

#include "IRenderer.h"

#include "CollisionDetection.h"


struct ModelData
{
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glm::mat4* view;    //  shared ptrs 
    glm::mat4* proj;

    std::string modelName;

    std::unique_ptr<Model> model;
    std::unique_ptr<ModelBridge> modelBridge;
};

class Scene
{
private:
    OpenGLRenderer& renderer;

    std::unordered_map<std::string, std::unique_ptr<Model>> models;
    std::unordered_map<std::string, std::unique_ptr<ModelBridge>> modelBridges;

    bool drawOctrees = true;
    bool drawLeafsOnly = true;
public:
    Scene(IRenderer* _renderer) : renderer(*static_cast<OpenGLRenderer*>(_renderer))
    {
        
        models["backpack"] = std::make_unique<Model>(
            "C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack",
            "\\backpack.obj"
        );

        auto mb = std::make_unique<ModelBridge>();
        mb->AttachModel(models["backpack"].get());
        modelBridges["backpack"] = std::move(mb);

        models["girl"] = std::make_unique<Model>(
            "C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\girl",
            "\\girl.obj"
        );
        
        auto mb2 = std::make_unique<ModelBridge>();
        mb2->AttachModel(models["girl"].get());
        modelBridges["girl"] = std::move(mb2);
    }

    // Delete copy/move because of reference member
    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    void UpdateOctree(std::string modelName, glm::vec3 deltaMovement)
    {
        models.find(modelName)->second->modelUpdated.RaiseEvent(deltaMovement);
    }

    void DrawModel(glm::mat4 &model, glm::mat4 &view, glm::mat4 &proj, std::string modelName)
    {

        renderer.UseShader("ModelShader");

        renderer.GetShader("ModelShader")->SetMat4("model", model);
        renderer.GetShader("ModelShader")->SetMat4("view", view);
        renderer.GetShader("ModelShader")->SetMat4("projection", proj);

        modelBridges.find(modelName)->second->Draw(*renderer.GetShader("BlockShader"));

        //  Updates models octree based on model matrix, ideally cache this so it isn't updated per frame
        //models.find(modelName)->second->modelUpdated.RaiseEvent(model);

        if (drawOctrees)
        {
            renderer.UseShader("OctreeShader");
            renderer.GetShader("OctreeShader")->SetMat4("model", model);
            renderer.GetShader("OctreeShader")->SetMat4("view", view);
            renderer.GetShader("OctreeShader")->SetMat4("projection", proj);

            if (drawLeafsOnly)
                modelBridges.find(modelName)->second->DrawOctreeLeafs(*renderer.GetShader("OctreeShader"));
            else 
                modelBridges.find(modelName)->second->DrawOctree(*renderer.GetShader("OctreeShader"));
        }
    }

    void Draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &proj)
    {
        //  Have a vector of ModelData and draw every single one in here
        //  Check if any of the models intersect
    }

    void CheckIntersections()
    {
        if (!Collision::CheckCollision(models["backpack"]->octree.get(), models["girl"]->octree.get()).empty())
            std::cout << "COLLIDING \n";
        else
            std::cout << "NOT COLLIDING \n";
    }
};



#endif