#ifndef _INTERSECTION_LOGIC_H__
#define _INTERSECTION_LOGIC_H__

#include "Structs.h"

/*
    File for general intersction detection between two shapes 
    of unknown dimensions / complexity. Achieved through the use
    of Separation Axis Theory. Currently only supports triangle
    and cube intersection and is highly simplified due to computation
    speeds.
*/

static std::pair<float, float> ProjectOnAxis(const glm::vec3 &axis, const std::vector<glm::vec3> &vertices)
{
    float p = glm::dot(vertices[0], axis);
    float minP = p, maxP = p;

    for (size_t i = 1; i < vertices.size(); i++)
    {
        p = glm::dot(vertices[i], axis);
        if (p < minP)
            minP = p;
        if (p > maxP)
            maxP = p;
    }

    return {minP, maxP};
}

static std::vector<glm::vec3> GetBoxVertices(const BoundingBox &cube)
{
    glm::vec3 v0(cube.min.x, cube.min.y, cube.min.z);
    glm::vec3 v1(cube.max.x, cube.min.y, cube.min.z);
    glm::vec3 v2(cube.max.x, cube.max.y, cube.min.z);
    glm::vec3 v3(cube.min.x, cube.max.y, cube.min.z);

    glm::vec3 v4(cube.min.x, cube.min.y, cube.max.z);
    glm::vec3 v5(cube.max.x, cube.min.y, cube.max.z);
    glm::vec3 v6(cube.max.x, cube.max.y, cube.max.z);
    glm::vec3 v7(cube.min.x, cube.max.y, cube.max.z);

    std::vector<glm::vec3> vertices = {v0, v1, v2, v3, v4, v5, v6, v7};
    return vertices;
}

static std::vector<glm::vec3> CalculateCubeEdges(const BoundingBox &cube)
{
    std::vector<glm::vec3> edges;

    // All 8 corners of the cube
    glm::vec3 v0(cube.min.x, cube.min.y, cube.min.z);
    glm::vec3 v1(cube.max.x, cube.min.y, cube.min.z);
    glm::vec3 v2(cube.max.x, cube.max.y, cube.min.z);
    glm::vec3 v3(cube.min.x, cube.max.y, cube.min.z);

    glm::vec3 v4(cube.min.x, cube.min.y, cube.max.z);
    glm::vec3 v5(cube.max.x, cube.min.y, cube.max.z);
    glm::vec3 v6(cube.max.x, cube.max.y, cube.max.z);
    glm::vec3 v7(cube.min.x, cube.max.y, cube.max.z);

    // 12 edges (v0->v1, v1->v2, etc.)
    edges.push_back(v1 - v0);
    edges.push_back(v2 - v1);
    edges.push_back(v3 - v2);
    edges.push_back(v0 - v3);

    edges.push_back(v5 - v4);
    edges.push_back(v6 - v5);
    edges.push_back(v7 - v6);
    edges.push_back(v4 - v7);

    edges.push_back(v4 - v0);
    edges.push_back(v5 - v1);
    edges.push_back(v6 - v2);
    edges.push_back(v7 - v3);

    return edges;
}

static std::vector<glm::vec3> CalculateTriangleEdges(const Triangle &tri)
{
    std::vector<glm::vec3> edges;

    glm::vec3 e1 = tri.v1->Position - tri.v0->Position;
    glm::vec3 e2 = tri.v2->Position - tri.v1->Position;
    glm::vec3 e3 = tri.v0->Position - tri.v2->Position;

    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    return edges;
}

//  Separation Axis Theory for cube and triangle 
static bool SAT(const BoundingBox& cube, const Triangle& tri)
{
    glm::vec3 cubeVerts[8] = {
        {cube.min.x, cube.min.y, cube.min.z},
        {cube.max.x, cube.min.y, cube.min.z},
        {cube.max.x, cube.max.y, cube.min.z},
        {cube.min.x, cube.max.y, cube.min.z},
        {cube.min.x, cube.min.y, cube.max.z},
        {cube.max.x, cube.min.y, cube.max.z},
        {cube.max.x, cube.max.y, cube.max.z},
        {cube.min.x, cube.max.y, cube.max.z},
    };
    glm::vec3 triVerts[3] = {
        tri.v0->Position, tri.v1->Position, tri.v2->Position
    };

    // All Axes to compare
    glm::vec3 axes[4] = {
        // Cube face normals (since axis-aligned)
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1),
        //  Triangle face
        glm::cross(tri.v1->Position - tri.v0->Position, tri.v2->Position - tri.v0->Position)
    };


    for (int i = 0; i < 4; i++)
    {
        glm::vec3 axis = axes[i];

        if (axis.length() < 1e-12f)
            continue;

        // Project cube
        float minCube = glm::dot(cubeVerts[0], axis);
        float maxCube = minCube;
        for (int i = 1; i < 8; ++i)
        {
            float p = glm::dot(cubeVerts[i], axis);
            if (p < minCube)
                minCube = p;
            if (p > maxCube)
                maxCube = p;
        }

        // Project triangle
        float minTri = glm::dot(triVerts[0], axis);
        float maxTri = minTri;
        for (int i = 1; i < 3; ++i)
        {
            float p = glm::dot(triVerts[i], axis);
            if (p < minTri)
                minTri = p;
            if (p > maxTri)
                maxTri = p;
        }

        // Separating axis?
        if (maxCube < minTri || maxTri < minCube)
            return false;   //  no intersection
    }


    return true;    // intersection
}


#endif