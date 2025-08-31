
#include "Mesh.h"

struct MeshData
{
    std::vector<Texture> textures;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
};

class MeshBridge
{
private:
    MeshData meshData;
public:
    MeshBridge(Mesh &mesh);

    void SetupMesh(const Mesh& mesh);
    void Draw(Shader& shader);
};