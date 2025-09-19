#ifndef _SCENE_H__
#define _SCENE_H__

#include <vector>
#include <unordered_map>

#include "Shapes/Model.h"
#include "ModelBridge.h"

#include "IRenderer.h"

#include "CollisionDetection.h"

using LeafPair = std::pair<Node *, Node *>;
using LeafPairs = std::vector<LeafPair>;

//  General structure for all data relating to a model
struct ModelData
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
    glm::mat4 transform = glm::mat4(1.0f);
    const glm::mat4 *view; //  shared ptrs
    const glm::mat4 *proj;

    std::string modelName;

    ModelData(std::string name, glm::mat4 _transform, glm::mat4* _view, glm::mat4* _proj) : modelName(name), transform(_transform), view(_view), proj(_proj)
    {
        std::string path = "C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\models\\" + name + "\\";
        std::string fileName = name + ".obj";

        model = std::make_unique<Model>( path.c_str(), fileName );
        modelBridge = std::make_unique<ModelBridge>();
        modelBridge->AttachModel(model.get());

        //  Updates position data that was already pregenerated from model
        prevPos = glm::vec3(transform[3]);
    }

    void Translate(float deltaTime, glm::vec3 translation)
    {
        transform = glm::translate(transform, translation * deltaTime);
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

    Octree* GetOctree()
    {
        return model.get()->octree.get();
    }

    void SetHidden(bool hiddenState)
    {
        modelBridge.get()->hidden = hiddenState;
    }
};

using ModelMap = std::unordered_map<std::string, std::unique_ptr<ModelData>>;





class Scene
{
private:
    OpenGLRenderer& renderer;

    ModelMap models;
    std::unordered_map<std::string, LeafPairs> modelBlend;

    void AddModel(std::string name, glm::mat4 _trans, glm::mat4* _view, glm::mat4* _proj)
    {
        //  Must have a correct file path of \\name\\name.obj
        models[name] = std::make_unique<ModelData>(name, _trans, _view, _proj);
    }

public:
    Scene(IRenderer* _renderer, ICamera* _cam, IInput* _input) : renderer(*static_cast<OpenGLRenderer*>(_renderer))
    {
        auto view = _cam->GetViewMatrixPtr();
        auto proj = _cam->GetProjMatrixPtr();

        AddModel("backpack", glm::mat4(1.0f), view, proj);
        AddModel("girl", glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, -0.5f)), view, proj);

        //  Update octree if it moves, same with model pos
        std::function<void()> leftMove = [&] { models["backpack"]->Translate(*renderer.GetDeltaTime(), glm::vec3(-2.0, 0, 0)); };
        std::function<void()> rightMove = [&] { models["backpack"]->Translate(*renderer.GetDeltaTime(), glm::vec3(2.0, 0, 0)); };
        std::function<void()> forwardMove = [&] { models["backpack"]->Translate(*renderer.GetDeltaTime(), glm::vec3(0, 0, -2.0)); };
        std::function<void()> backwardMove = [&] { models["backpack"]->Translate(*renderer.GetDeltaTime(), glm::vec3(0, 0, 2.0)); };

        _input->AttachKeyEvent(leftMove, GLFW_KEY_LEFT);
        _input->AttachKeyEvent(rightMove, GLFW_KEY_RIGHT);
        _input->AttachKeyEvent(forwardMove, GLFW_KEY_UP);
        _input->AttachKeyEvent(backwardMove, GLFW_KEY_DOWN);
    }

    // Delete copy/move because of reference member
    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    void CheckIntersections()
    {
        //  Octrees are messed up for girl
        auto &nodes = Collision::CheckCollision(models["backpack"].get()->GetOctree(), models["girl"].get()->GetOctree());
        //modelBlend["backpackgirl"] = nodes;

        //  Not hidden until collision occurs
        models["backpack"]->SetHidden(false);
        models["girl"]->SetHidden(false);
        for (auto &node : nodes)
        {
            node.first->onIntersection.RaiseEvent(true);
            node.second->onIntersection.RaiseEvent(true);

            //  Hide model and then generate new model (MAKE THIS EVENT DRIVEN)
            //models["backpack"]->SetHidden(true);
            //models["girl"]->SetHidden(true);
        }
    }

    void Update()
    {
        //  If objects intersect they will not be drawn
        for (auto& model : models)
        {
            model.second->Draw(renderer);
        }

        //  If Intersected it will instead draw their blended model
        for (auto &node : modelBlend["backpackgirl"])
        {
            //node->Draw(*renderer.GetShader("BlockShader"));
        }
    }
};



#endif