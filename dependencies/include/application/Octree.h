#ifndef _OCTREE_H__
#define _OCTREE_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

/*
    Octree, 
    TODO: Make base class that divides space 2^n many times, instead 
    of hard coding an octree implementation

    TODO: Make a bridge that renders the octree based on the bounding boxes
*/

struct BoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};

class Node
{
private:
    Node* parent;
    std::vector<Node> children;

    int maxTreeDepth = 6;
    int currentDepth = 0;

public:
    BoundingBox bounds;

    Node();
    Node(BoundingBox _bounds, int _currentDepth) : bounds(_bounds), currentDepth(_currentDepth) {}
    void DetermineChildren(BoundingBox _bounds, int _childrenCount);
};

class Octree : public Node
{
private: 
    std::vector<glm::vec3> modelVertices;
public:
    Octree(std::vector<glm::vec3> _modelvertices);
};


















//  TEMPORARILY here for now until we set up a bridge
class Cube
{
private:
    unsigned int VBO, EBO;

    BoundingBox bounds;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
public:
    unsigned int VAO;

    Cube(BoundingBox _bounds) : bounds(_bounds)
    { 
        SetupIndices();
        SetupVertices();
        SetupShapeMesh();
    }
    ~Cube()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    //  Drawn based on BoundingBox
    void SetupIndices()
    {
        indices.resize(36);
        indices = {
            // Bottom 
            0, 2, 3,
            3, 1, 0,

            // Top 
            4, 5, 7,
            7, 6, 4,

            // Front
            1, 3, 7,
            7, 5, 1,

            // Back 
            0, 4, 6,
            6, 2, 0,

            // Left 
            0, 1, 5,
            5, 4, 0,

            // Right 
            2, 6, 7,
            7, 3, 2};
    }
    void SetupVertices()
    {
        //  Bottom Left Back
        vertices.emplace_back(bounds.min);
        //  Bottom Left Front
        vertices.emplace_back(bounds.min.x, bounds.min.y, bounds.max.z);
        //  Bottom Right Back
        vertices.emplace_back(bounds.max.x, bounds.min.y, bounds.min.z);
        //  Bottom Right Front
        vertices.emplace_back(bounds.max.x, bounds.min.y, bounds.max.z);

        //  Top Left Back
        vertices.emplace_back(bounds.min.x, bounds.max.y, bounds.min.z);
        //  Top Left Front
        vertices.emplace_back(bounds.min.x, bounds.max.y, bounds.max.z);
        //  Top Right Back
        vertices.emplace_back(bounds.max.x, bounds.max.y, bounds.min.z);
        //  Top Right Front
        vertices.emplace_back(bounds.max);
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

        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

        glBindVertexArray(0);
    }

    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};



#endif