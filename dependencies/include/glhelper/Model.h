#ifndef __MODEL_H__
#define __MODEL_H__

#include <utility>

#include <glm/glm.hpp>

#include "Shader.h"
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

class Mesh {
private:
    unsigned int VBO, EBO;
    void SetupMesh();
public:
    unsigned int VAO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
    void Draw(Shader &shader);
};

class Model
{
private:
    std::vector<glm::vec3> vertexPositions;
    std::vector<Texture> textures_loaded; 
    std::vector<Mesh> meshes;
    std::string directory;

    void LoadModel(std::string path, std::string objName);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
public:
    std::unique_ptr<Octree> octree = nullptr;
    std::vector<Cube> octreeRender = {};

    Model(char *path, std::string objName);

    std::vector<Mesh> GetMeshes() { return meshes; }
    std::vector<glm::vec3> GetVertices();

    void Draw(Shader &shader);
    void DrawOctree(Shader &shader);    //  Not implemented yet
};


#endif 