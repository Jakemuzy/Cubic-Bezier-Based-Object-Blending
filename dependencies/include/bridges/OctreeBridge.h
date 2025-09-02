#ifndef _BOUNDING_BOX_RENDER_H__
#define _BOUNDING_BOX_RENDER_H__

#include <vector>

#include "Shader.h"
#include "Octree.h"
#include "OpenGLRenderer.h"

//  TEMPORARILY here for now until we set up a bridge
class OctreeBridge
{
private:
    int depth = 1;
    unsigned int VBO, EBO;

    BoundingBox bounds;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

public:
    unsigned int VAO;

    OctreeBridge(BoundingBox _bounds, int _depth) : bounds(_bounds), depth(_depth)
    {
        SetupVertices();
        SetupIndices();
        SetupShapeMesh();
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
            6, 2, 1
        };
    }

    void SetupVertices()
    {
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

    void Draw(IShader& shader)
    {
        shader.SetInt("currentDepth", depth);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


#endif