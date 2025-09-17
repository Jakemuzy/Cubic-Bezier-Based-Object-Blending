#ifndef _JMESH_H__
#define _JMESH_H__

#include <utility>

#include <glm/glm.hpp>

#include "Shader.h"
#include "EventHandler.h"

#include "Octree.h"

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONE_INFLUENCE 4

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Mesh
{
private:
    void SetupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);

    void SetupTriangles();

    std::vector<Vertex>& GetVertices() { return vertices; }
    std::vector<Triangle>& GetTriangles() { return triangles; }
    std::vector<unsigned int>& GetIndices() { return indices; }
    std::vector<Texture>& GetTextures() { return textures; }
};

#endif