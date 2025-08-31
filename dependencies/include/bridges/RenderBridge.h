#ifndef _RENDER_BRIDGE_PROVIDER_H__
#define _RENDER_BRIDGE_PROVIDER_H__

struct RenderData
{
    /*
    std::vector<Vertex> cachedVertices;
    std::vector<unsigned int> cachedIndices;
    std::vector<Texture> cahcedTextures;
    */

    unsigned int VAO, VBO, EBO;
    // GPU handles (VBO/VAO/EBO) if needed
};

class RenderBridge
{

};

#endif

