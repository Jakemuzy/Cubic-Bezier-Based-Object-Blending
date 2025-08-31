#ifndef _JMESH_H__
#define _JMESH_H__

#include <utility>

#include <glm/glm.hpp>

#include "Shader.h"
#include "ServiceProvider.h"
#include "EventHandler.h"

#include "Octree.h"

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONE_INFLUENCE 4

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
private:
    void SetupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);

    std::vector<Vertex>& GetVertices() { return vertices; }
    std::vector<unsigned int>& GetIndices() { return indices; }
    std::vector<Texture>& GetTextures() { return textures; }
};

#endif