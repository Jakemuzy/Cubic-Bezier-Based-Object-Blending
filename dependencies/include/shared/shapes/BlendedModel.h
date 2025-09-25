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
ClosestTriangle FindClosestTriangle(Triangle& tri, std::vector<Triangle>* otherTris);

//  Finds the Closest Point on a triangle to a given point and returns info about it 
//  https://gdbooks.gitbooks.io/3dcollisions/content/Chapter4/closest_point_to_triangle.html
ClosestTriangle ClosestPointOnTriangle(const glm::vec3& p, Triangle& tri);

class BlendedModel
{
private:
    ModelData* modelA = nullptr;
    ModelData* modelB = nullptr;

    LeafPairs* CollidingNodes;

    Model modelBlend;
    ModelBridge modelBlendBridge;
    
    float blendFactor = 0.5f;
    glm::vec3 prevPosMA, prevPosMB;

    std::unique_ptr<Octree> octree = nullptr;

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
            if (glm::length(newA - prevPosMA) > 0.1f)
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

        ClosestTriangle tri;

        for(auto& pair : *CollidingNodes)
        {
            pair.first->triangles;
            for (auto& nodeOneTri : *(pair.first->triangles))
            {
                ClosestTriangle closest = FindClosestTriangle(nodeOneTri, pair.second->triangles);
                
                glm::vec3 currentBlend = glm::mix(nodeOneTri.v0->Position, closest.barycentric.x * closest.tri->v0->Position +
                                                   closest.barycentric.y * closest.tri->v1->Position +
                                                   closest.barycentric.z * closest.tri->v2->Position, blendFactor);
            }
        }


        /*
                
        /*
        MATCH FOUND
            If a match is found, iterate through all triangles in LeafA, finding the nearest neighbor triangle in LeafB
            Once this nearest neighbor is found, project all vertices in TriA onto TriB
            Once vertices projected onto TriB, compute the barycentric coordinates of the vertices on TriB
            After Barycentric coordinates are found use the original vertA and the barycentric to determine the new
            position, normal, and color, etc
        */


        /*
            Okay so we use closest point on a triangle to a point.
            And based on this closest point on a triangle, we determine which triangle is closest to the vertex.
            We then project the point onto said triangle, getting the barycentric coordinates of the projection

            So you get the barycentric coordinates of the point projected on the triangle.
            Based on this barycentric coordinates, you do a formula with the original point, and 
            find out how to blend the original point.
        */
    }
};


#endif