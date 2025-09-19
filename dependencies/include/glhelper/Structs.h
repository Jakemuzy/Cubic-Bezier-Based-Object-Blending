#ifndef _STRUCTS_H__
#define _STRUCTS_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


//  Raw data structs

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Triangle
{
    Vertex *v0;
    Vertex *v1;
    Vertex *v2;
    Triangle(Vertex &_v0, Vertex &_v1, Vertex &_v2) : v0(&_v0), v1(&_v1), v2(&_v2) {}
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

struct BoundingBox
{
    glm::vec3 min;
    glm::vec3 max;
};

//  


#endif 