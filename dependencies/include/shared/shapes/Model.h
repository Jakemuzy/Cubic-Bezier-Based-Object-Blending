#ifndef __JMODEL_H__
#define __JMODEL_H__

#include "Shapes/Mesh.h"

/*
    The purpose of this class is to SOLELY convert assimp's vertices
    and indices into something that opengl can understand, it ONLY
    holds the data, doesn't contain any rendering logic imported 
    from assimp. Each model contains many mesh's
*/

class Model
{
private:
    std::vector<Texture> textures_loaded; 
    std::vector<Mesh> meshes;
    std::string directory;

    void LoadModel(std::string path, std::string objName);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
public:
    Event<glm::vec3> modelUpdated;
    //  Realistically should be shared ptr
    std::unique_ptr<Octree> octree = nullptr;

    Model(const char *path, std::string objName);

    std::vector<Mesh>& GetMeshes() { return meshes; }
    
    void Model::InitalizeOctree();
};


#endif 