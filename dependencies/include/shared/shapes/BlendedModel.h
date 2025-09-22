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

//  Project point on closest triangle in order to find barycentric coords and blend
//  https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
void ProjectPointOnTri(Triangle& tri, std::vector<Triangle*> otherTris)
{
    ClosestTriangle(tri, otherTris);
};

//  Finds the closest TriangleB to given TriangleA
Triangle* ClosestTriangle(Triangle& tri, std::vector<Triangle*> otherTris)
{
    float closestDist = std::numeric_limits<float>::max();
    Triangle* closest = nullptr;

    for(auto& otherTri : otherTris)
    {
        float dist1 = ClosestPointOnTriangle(tri.v0->Position, otherTri);
        if (dist1 < closestDist)
        {
            closestDist = dist1;
            closest = otherTri;
        }

        float dist2 = ClosestPointOnTriangle(tri.v1->Position, otherTri);
        if (dist2 < closestDist)
        {
            closestDist = dist2;
            closest = otherTri;
        }

        float dist3 = ClosestPointOnTriangle(tri.v2->Position, otherTri);
        if (dist3 < closestDist)
        {
            closestDist = dist3;
            closest = otherTri;
        }
    }

    return closest;
};

//  Finds the Closest Point on a triangle to a given point and gets the distance 
//  https://gdbooks.gitbooks.io/3dcollisions/content/Chapter4/closest_point_to_triangle.html
glm::vec3 ClosestPointOnTriangle(const glm::vec3& p, const Triangle* tri)
{
    glm::vec3 a = tri->v0->Position;
    glm::vec3 b = tri->v1->Position;
    glm::vec3 c = tri->v2->Position;

    //  Edges
    glm::vec3 ba = b - a;
    glm::vec3 ca = c - a;
    glm::vec3 cb = c - b;

    //  Replacements for cross products (cuz expensive)
    float d1 = glm::dot(b - a, p - a);
    float d2 = glm::dot(c - a, p - a);
    float d3 = glm::dot(b - a, p - b);
    float d4 = glm::dot(c - a, p - b);
    float d5 = glm::dot(b - a, p - c);
    float d6 = glm::dot(c - a, p - c);

    float snom = d1;
    float sdenom = -d3;

    float tnom = d2;
    float tdenom = -d6;

    //  Vertex region early out
    if (snom <= 0.0f && tnom <= 0.0f) 
        return a;

    //  Parametric pos u for projection P' of P on BC
    float unom = d4 - d3;
    float udenom = d5 - d6;

    //  Vertex region early out
    if (sdenom <= 0.0f && unom <= 0.0f)
        return b; 
    if (tdenom <= 0.0f && udenom <= 0.0f)
        return c;

    //  P outside (or on) AB if triple scalar product [N PA PB] <= 0
    float vc = d1 * d4 - d3 * d2;

    //  P outside AB and within feature region of AB
    //  return projection of P onto AB
    if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
        return a + snom / (snom + sdenom) * ba;

    //  P outside (or on) BC if triple scalar product [N PB PC] <= 0
    float va = d3 * d6 - d5 * d4;

    // P outside BC and within feature region of BC,
    // return projection of P onto BC
    if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f) 
        return b + unom / (unom + udenom) * cb;

    //  P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
    float vb = d5 * d2 - d1 * d6;
    // P outside CA and within feature region of CA,
    // return projection of P onto CA
    if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
        return a + tnom / (tnom + tdenom) * ca;

    // P must project inside face region. Compute Q using barycentric coordinates
    float u = va / (va + vb + vc);
    float v = vb / (va + vb + vc);
    float w = 1.0f - u - v; // = vc / (va + vb + vc)

    //  We only need the barycentric coordinates
    //return u * a + v * b + w * c;
    return glm::vec3(u, v, w);
};

glm::vec3 ClosestVertexOnTri(const glm::vec3 &p, const Triangle* tri)
{
    //  Vectors pointing from each vertex to point
    glm::vec3 edge1 = p - tri->v0->Position;
    glm::vec3 edge2 = p - tri->v1->Position;
    glm::vec3 edge3 = p - tri->v2->Position;

    //  Distances
    float dist1 = edge1.length();
    float dist2 = edge2.length();
    float dist3 = edge3.length();

    //  Shortest length
    float min = (dist1 < dist2 ? dist1 : dist2);
    min = min < dist3 ? min : dist3;

    if (min == dist1)
        return tri->v0;
    else if (min == dist2)
        return tri->v1;
    return tri->v2;
}

class BlendedModel
{
private:
    ModelData* modelA = nullptr;
    ModelData* modelB = nullptr;

    LeafPairs* CollidingNodes;

    Model modelBlend;
    ModelBridge modelBlendBridge;

    std::unique_ptr<Octree> octree = nullptr;
    Event<void()> onUpdateBlend;

public:
    BlendedModel() = default;

    //  Instead of Model this should be ModelData
    BlendedModel(ModelData* a, ModelData* b, LeafPairs& lp) : modelA(a), modelB(b), CollidingNodes(&lp) { }

    //  For each node that intersects in the octree, get all the vertices in the node,
    //  https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
    //  https://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf 
    void UpdateBlend()
    {
        /*
        Iterate octrees of both models at the same time

        compare to see if any leafpair memory address == the models memory address
        if they do match, then blend the nearest vertices

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