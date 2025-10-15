#include "shapes/BlendedModel.h"

//  Finds the closest TriangleB to given TriangleA
ClosestTriangle FindClosestTriangle(Triangle &tri, std::vector<Triangle> &otherTris)
{
    float closestDist = std::numeric_limits<float>::max();
    ClosestTriangle closest;

    for (auto &otherTri : otherTris)
    {
        ClosestTriangle tri1 = ClosestPointOnTriangle(tri.v0->Position, otherTri);
        if (tri1.distance < closestDist)
        {
            closest = tri1;
            closestDist = tri1.distance;
        }

        ClosestTriangle tri2 = ClosestPointOnTriangle(tri.v1->Position, otherTri);
        if (tri2.distance < closestDist)
        {
            closest = tri2;
            closestDist = tri2.distance;
        }

        ClosestTriangle tri3 = ClosestPointOnTriangle(tri.v2->Position, otherTri);
        if (tri3.distance < closestDist)
        {
            closest = tri3;
            closestDist = tri3.distance;
        }
    }

    return closest;
};

ClosestTriangle ClosestPointOnTriangle(const glm::vec3 &p, Triangle &tri)
{
    glm::vec3 a = tri.v0->Position;
    glm::vec3 b = tri.v1->Position;
    glm::vec3 c = tri.v2->Position;

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
    {
        float dist = glm::length(p - a);
        ClosestTriangle close(a, glm::vec3(1.0f, 0.0f, 0.0f), &tri, dist);
        return close;
    }

    //  Parametric pos u for projection P' of P on BC
    float unom = d4 - d3;
    float udenom = d5 - d6;

    //  Vertex region early out
    if (sdenom <= 0.0f && unom <= 0.0f)
    {
        float dist = glm::length(p - b);
        ClosestTriangle close(b, glm::vec3(0.0f, 1.0f, 0.0f), &tri, -1.0f);
        return close;
    }
    if (tdenom <= 0.0f && udenom <= 0.0f)
    {
        float dist = glm::length(p - c);
        ClosestTriangle close(c, glm::vec3(0.0f, 0.0f, 1.0f), &tri, -1.0f);
        return close;
    }

    //  P outside (or on) AB if triple scalar product [N PA PB] <= 0
    float vc = d1 * d4 - d3 * d2;

    //  P outside AB and within feature region of AB
    //  return projection of P onto AB
    if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
    {
        ClosestTriangle close(a + snom / (snom + sdenom) * ba, glm::vec3(-1.0f), &tri, -1.0f);
        return close;
    }
    //  P outside (or on) BC if triple scalar product [N PB PC] <= 0
    float va = d3 * d6 - d5 * d4;

    // P outside BC and within feature region of BC,
    // return projection of P onto BC
    if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
    {
        ClosestTriangle close(b + unom / (unom + udenom) * cb, glm::vec3(-1.0f), &tri, -1.0f);
        return close;
    }

    //  P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
    float vb = d5 * d2 - d1 * d6;
    // P outside CA and within feature region of CA,
    // return projection of P onto CA
    if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
    {
        ClosestTriangle close(a + tnom / (tnom + tdenom) * ca, glm::vec3(-1.0f), &tri, -1.0f);
        return close;
    }

    // P must project inside face region. Compute Q using barycentric coordinates
    float u = va / (va + vb + vc);
    float v = vb / (va + vb + vc);
    float w = 1.0f - u - v; // = vc / (va + vb + vc)

    float dist = glm::length(p - (u * a + v * b + w * c));
    ClosestTriangle close((u * a + v * b + w * c), glm::vec3(u, v, w), &tri, dist);
    return close;
};
