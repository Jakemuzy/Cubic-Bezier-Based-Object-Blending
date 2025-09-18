#ifndef _BOUNDING_BOX_RENDER_H__
#define _BOUNDING_BOX_RENDER_H__

#include <vector>
#include <memory>

#include "Shader.h"
#include "OpenGLRenderer.h"
#include "shapes/Octree.h"

//  TEMPORARILY here for now until we set up a bridge
class NodeBridge
{
private:
    unsigned int VBO, EBO;
    std::vector<std::unique_ptr<NodeBridge>> children;

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    int depth;
    bool intersectedLastFrame = false;
public:
    bool intersected = false;

    unsigned int VAO;

    NodeBridge(Node* node, int _depth) : depth(_depth)
    {
        SetupVertices(node);
        SetupIndices();
        SetupShapeMesh();

        for (auto& child : node->GetChildren())
        {
            children.push_back(std::make_unique<NodeBridge>(child.get(), depth + 1));
        }

        //  Intersection Event
        node->onIntersection.Subscribe([&](bool state) {
             intersected = state; 
             intersectedLastFrame = state;
        });
    }

    ~NodeBridge()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    //  Drawn based on BoundingBox
    void SetupIndices()
    {
        indices = {
            // Bottom
            0, 1, 2,
            2, 3, 0,

            // Top
            4, 7, 6,
            6, 5, 4,

            // Front
            3, 2, 6,
            6, 7, 3,

            // Back
            0, 4, 5,
            5, 1, 0,

            // Left
            0, 3, 7,
            7, 4, 0,

            // Right
            1, 5, 6,
            6, 2, 1};
    }

    void SetupVertices(Node* node)
    {
        BoundingBox bounds = node->bounds;

        // Bottom (y = min.y)
        vertices.emplace_back(bounds.min.x, bounds.min.y, bounds.min.z); // 0 left-back
        vertices.emplace_back(bounds.max.x, bounds.min.y, bounds.min.z); // 1 right-back
        vertices.emplace_back(bounds.max.x, bounds.min.y, bounds.max.z); // 2 right-front
        vertices.emplace_back(bounds.min.x, bounds.min.y, bounds.max.z); // 3 left-front

        // Top (y = max.y)
        vertices.emplace_back(bounds.min.x, bounds.max.y, bounds.min.z); // 4 left-back
        vertices.emplace_back(bounds.max.x, bounds.max.y, bounds.min.z); // 5 right-back
        vertices.emplace_back(bounds.max.x, bounds.max.y, bounds.max.z); // 6 right-front
        vertices.emplace_back(bounds.min.x, bounds.max.y, bounds.max.z); // 7 left-front
    }

    void SetupShapeMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

        glBindVertexArray(0);
    }

    void Draw(IShader &shader)
    {
        // Draw this node
        shader.SetInt("currentDepth", depth);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw all children recursively
        for (auto &child : children)
        {
            child->Draw(shader);
        }

        //  Collision visual logic
        if(intersectedLastFrame)
        {
            intersected = intersectedLastFrame = false;
        }
    }

    void DrawLeafs(IShader& shader)
    {
        // Draw this node
        if(children.empty())
        {
            shader.SetInt("currentDepth", depth);
            shader.SetBool("intersected", intersected);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        // Draw all children recursively
        for (auto &child : children)
        {
            child->DrawLeafs(shader);
        }

        //  Collision visual logic
        if (intersectedLastFrame)
        {
            intersected = intersectedLastFrame = false;
        }
    }
};

class OctreeBridge
{
private:
    std::unique_ptr<NodeBridge> rootNodeBridge;
public:
    
    OctreeBridge() = default;
    OctreeBridge(Octree* octree)
    {
        rootNodeBridge = std::make_unique<NodeBridge>(octree->GetRoot(), 1);
    }

    void Draw(IShader& shader)
    {
        if (rootNodeBridge)
        {
            rootNodeBridge->Draw(shader);
        }
    }

    //  Draw leaf nodes only
    void DrawLeafs(IShader& shader)
    {
        if(rootNodeBridge)
        {
            rootNodeBridge->DrawLeafs(shader);
        }
    }
};


#endif