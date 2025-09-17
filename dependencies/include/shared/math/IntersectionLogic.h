#ifndef _INTERSECTION_LOGIC_H__
#define _INTERSECTION_LOGIC_H__

#include "Structs.h"

/*
    File for general intersction detection between two shapes 
    of unknown dimensions / complexity. Achieved through the use
    of Separation Axis Theory.
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
    // Collect vertices from both structures
    std::vector<glm::vec3> cubeVerts = GetBoxVertices(cube); 
    std::vector<glm::vec3> triVerts = {tri.v0->Position, tri.v1->Position, tri.v2->Position};

    // All Axes to compare
    std::vector<glm::vec3> axes;

    // Cube face normals (since axis-aligned)
    axes.push_back(glm::vec3(1, 0, 0));
    axes.push_back(glm::vec3(0, 1, 0));
    axes.push_back(glm::vec3(0, 0, 1));

    // Triangle face normal
    glm::vec3 triNormal = glm::cross(tri.v1->Position - tri.v0->Position,
                                     tri.v2->Position - tri.v0->Position);
    if (glm::length(triNormal) > 1e-6f) 
        axes.push_back(triNormal);

    // Cross Products not really necessary for octrees
    /*
    auto cubeEdges = CalculateCubeEdges(cube);
    auto triEdges = CalculateTriangleEdges(tri);
    for (auto &ec : cubeEdges)
    {
        for (auto &et : triEdges)
        {
            glm::vec3 axis = glm::cross(ec, et);
            if (glm::length(axis) > 1e-6f)
                axes.push_back(glm::normalize(axis));
        }
    }
    */

    // Test all the axis's 
    for (auto &axis : axes)
    {
        auto [minCube, maxCube] = ProjectOnAxis(axis, cubeVerts);
        auto [minTri, maxTri] = ProjectOnAxis(axis, triVerts);

        if (maxCube < minTri || maxTri < minCube)
        {
            // separating axis found so no collision
            return false;
        }
    }

    // no separating axis found so intersection
    return true;
}


#endif