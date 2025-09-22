#ifndef _MODEL_DATA__
#define _MODEL_DATA__

#include <memory>
#include <filesystem>

#include <shapes/Octree.h>
#include <shapes/Model.h>
#include <ModelBridge.h>

//  General structure for all data relating to a model
class ModelData
{
private:
    glm::vec3 prevPos;

    std::unique_ptr<Model> model;
    std::unique_ptr<ModelBridge> modelBridge;

    bool drawOctree = true, drawLeafNodesOnly = true;

    void UpdateOctree()
    {
        glm::vec3 currentPos = glm::vec3(transform[3]); // extract translation
        glm::vec3 deltaMovement = currentPos - prevPos;

        model->modelUpdated.RaiseEvent(deltaMovement);
        prevPos = currentPos;
    }

public:
    glm::mat4 transform;
    const glm::mat4 *view; //  shared ptrs
    const glm::mat4 *proj;

    std::string modelName;

    ModelData(std::string name, glm::mat4 _transform, glm::mat4 *_view, glm::mat4 *_proj) : modelName(name), transform(_transform), view(_view), proj(_proj)
    {
        std::filesystem::path path = std::filesystem::current_path() / "models" / name;
        std::string fileName = "\\" + name + ".obj";

        model = std::make_unique<Model>(path.string().c_str(), fileName);
        modelBridge = std::make_unique<ModelBridge>();
        modelBridge->AttachModel(model.get());

        //  Updates position data that was already pregenerated from model
        prevPos = glm::vec3(transform[3]);
        model->modelUpdated.RaiseEvent(transform[3]);
    }

    void Translate(float deltaTime, glm::vec3 translation)
    {
        transform = glm::translate(transform, deltaTime * translation);
        UpdateOctree();
    }

    void Draw(OpenGLRenderer &renderer)
    {
        renderer.UseShader("ModelShader");

        renderer.GetShader("ModelShader")->SetMat4("model", transform);
        renderer.GetShader("ModelShader")->SetMat4("view", *view);
        renderer.GetShader("ModelShader")->SetMat4("projection", *proj);

        modelBridge->Draw(*renderer.GetShader("ModelShader"));

        if (drawOctree)
        {
            renderer.UseShader("OctreeShader");
            renderer.GetShader("OctreeShader")->SetMat4("model", transform);
            renderer.GetShader("OctreeShader")->SetMat4("view", *view);
            renderer.GetShader("OctreeShader")->SetMat4("projection", *proj);

            if (drawLeafNodesOnly)
                modelBridge->DrawOctreeLeafs(*renderer.GetShader("OctreeShader"));
            else
                modelBridge->DrawOctree(*renderer.GetShader("OctreeShader"));
        }
    }

    Octree *GetOctree()
    {
        return model.get()->octree.get();
    }

    void SetHidden(bool hiddenState)
    {
        modelBridge.get()->hidden = hiddenState;
    }
};

#endif