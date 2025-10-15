#ifndef _BLENDED_MODEL_H__
#define _BLENDED_MODEL_H__

#include <limits>

#include "EventHandler.h"

#include "ModelData.h"

using LeafPair = std::pair<Node *, Node *>;
using LeafPairs = std::vector<LeafPair>;

/*
    This is the new model created from the two blended models.
    The manner in which this is done is essentially to copy all the 
    textures and meshes from the original model, however if any of the meshes
    intersect eachother, create a new one based upon the two intersecting
    ones. From there just copy the existing Models original mesh's and add
    the new ones.
*/

struct ClosestTriangle
{
    glm::vec3 pos;
    glm::vec3 barycentric;

    Triangle* tri;
    float distance;

    ClosestTriangle() {} 
    ClosestTriangle(glm::vec3& p, glm::vec3 b, Triangle* t, float d) : pos(p), barycentric(b), tri(t), distance(d) { } 
};

//  Finds the closest TriangleB to given TriangleA
ClosestTriangle FindClosestTriangle(Triangle& tri, std::vector<Triangle>& otherTris);

//  Finds the Closest Point on a triangle to a given point and returns info about it 
//  https://gdbooks.gitbooks.io/3dcollisions/content/Chapter4/closest_point_to_triangle.html
ClosestTriangle ClosestPointOnTriangle(const glm::vec3& p, Triangle& tri);

//  Hashing algorithm for identifying unique indices (not mine)
struct Vec3Hasher
{
    std::size_t operator()(const glm::vec3 &v) const
    {
        // Combine hashes of x, y, z 
        std::size_t hx = std::hash<float>{}(v.x);
        std::size_t hy = std::hash<float>{}(v.y);
        std::size_t hz = std::hash<float>{}(v.z);
        return hx ^ (hy << 1) ^ (hz << 2); // Combine 
    }
};

class BlendedModel
{
private:
    ModelData* modelA = nullptr;
    ModelData* modelB = nullptr;

    LeafPairs* CollidingNodes = nullptr;

    Model modelBlend;
    ModelBridge modelBlendBridge;
    
    float blendFactor = 0.5f;
    glm::vec3 prevPosMA, prevPosMB;

    std::unique_ptr<Octree> octree = nullptr;

    // TESTING
    unsigned int VAO, VBO, EBO;
    std::vector<glm::vec3> newVerts;
    std::vector<unsigned int> newIndices;
    std::unordered_map<glm::vec3, unsigned int, Vec3Hasher> indices;
    int frameCounter = 0;

public:
    Event<> onUpdateBlend;

    BlendedModel() = default;

    BlendedModel(ModelData* a, ModelData* b, LeafPairs& lp) : modelA(a), modelB(b), CollidingNodes(&lp) 
    { 
        prevPosMA = a->transform[3];
        prevPosMB = b->transform[3];

        //  Only update the model IF the model has moved enough distance
        onUpdateBlend.Subscribe([&] {
            glm::vec3 newA = a->transform[3];;
            if (glm::length(newA - prevPosMA) > 0.3f)
            {
                prevPosMA = newA;
                UpdateBlend();
            }
        });
    }

    //  For each node that intersects in the octree, get all the vertices in the node,
    //  https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
    //  https://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf 
    void UpdateBlend()
    {

        newVerts.clear();
        newIndices.clear();
        indices.clear();

        //  Copy meshses that aren't in colliding nodes 

        //  Blend meshes in colliding nodes
        for(auto& pair : *CollidingNodes)
        {

            for (auto &nodeOneTri : pair.first->triangles)
            {
                ClosestTriangle closest = FindClosestTriangle(nodeOneTri, pair.second->triangles);

                glm::vec3 blendV0 = glm::mix(nodeOneTri.v0->Position, closest.barycentric.x * closest.tri->v0->Position +
                                                   closest.barycentric.y * closest.tri->v1->Position +
                                                   closest.barycentric.z * closest.tri->v2->Position, blendFactor);

                glm::vec3 blendV1 = glm::mix(nodeOneTri.v1->Position, closest.barycentric.x * closest.tri->v0->Position +
                                                   closest.barycentric.y * closest.tri->v1->Position +
                                                   closest.barycentric.z * closest.tri->v2->Position, blendFactor);

                glm::vec3 blendV2 = glm::mix(nodeOneTri.v2->Position, closest.barycentric.x * closest.tri->v0->Position +
                                                   closest.barycentric.y * closest.tri->v1->Position +
                                                   closest.barycentric.z * closest.tri->v2->Position, blendFactor);

    
                auto AddVertex = [&](glm::vec3& vert) -> unsigned int
                {
                    auto it = indices.find(vert);
                    if(it != indices.end())
                        return it->second;
                    
                    unsigned int newIndex = (unsigned int)newVerts.size();
                    indices[vert] = newIndex;
                    newVerts.push_back(vert);

                    return newIndex;
                };

                newIndices.push_back(AddVertex(blendV0));
                newIndices.push_back(AddVertex(blendV1));
                newIndices.push_back(AddVertex(blendV2));
            }
        }

        // If leaf node not modified, copy data directly
        // Generate Vertex and Index data from blended triangles
            //  Already have vertices, put them into vertex array
            //  Deduplicate shared vertices
                //  Achieved via Hash map to track unique vertices
                //  If it already exists reuse index, if not add to vertex arrray with new index
        // Merge normals, uvs etc
        // Append to unmodified mesh

        // Check for continuity between blended and unblended regions
            // indecies should match up (later)

        /*
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (!newVerts.empty())
            glBufferData(GL_ARRAY_BUFFER, newVerts.size() * sizeof(glm::vec3), newVerts.data(), GL_STATIC_DRAW);
        else
            glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // safe empty buffer

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        if (!newIndices.empty())
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(unsigned int), newIndices.data(), GL_STATIC_DRAW);
        else
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);
        */
    }

    void Draw()
    {
        frameCounter++;
        if (frameCounter % 60 == 0) // every ~1 second at 60 FPS
        {
            std::cout << "Verts: " << newVerts.size()
                      << "  Indices: " << newIndices.size() << std::endl;
        }
        /*
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(newIndices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        */
    }

};


#endif