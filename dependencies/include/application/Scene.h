#ifndef _SCENE_H__
#define _SCENE_H__

#include <vector>
#include <filesystem>
#include <unordered_map>

#include "shapes/ModelData.h"
#include "shapes/BlendedModel.h"

#include "IRenderer.h"
#include "ICamera.h"
#include "IInput.h"

#include "CollisionDetection.h"

using LeafPair = std::pair<Node *, Node *>;
using LeafPairs = std::vector<LeafPair>;
using ModelMap = std::unordered_map<std::string, std::unique_ptr<ModelData>>;

class Scene
{
private:
    OpenGLRenderer& renderer;

    ModelMap models;
    std::unique_ptr<BlendedModel> modelBlend = nullptr;
    LeafPairs CollidingNodes;

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

        modelBlend = std::make_unique<BlendedModel>(models["backpack"].get(), models["girl"].get(), CollidingNodes);
    }

    // Delete copy/move because of reference member
    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    void CheckIntersections()
    {
        //  Octrees are messed up for girl
        auto nodes = Collision::CheckCollision(models["backpack"].get()->GetOctree(), models["girl"].get()->GetOctree());
        CollidingNodes = nodes;

        //  Not hidden until collision occurs
        models["backpack"]->SetHidden(false);
        models["girl"]->SetHidden(false);
        for (auto &node : nodes)
        {
            node.first->onIntersection.RaiseEvent(true);
            node.second->onIntersection.RaiseEvent(true);

            modelBlend->onUpdateBlend.RaiseEvent();

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
        for (auto &node : CollidingNodes)
        {
            //modelBlend->Draw(renderer);
        }
    }
};



#endif