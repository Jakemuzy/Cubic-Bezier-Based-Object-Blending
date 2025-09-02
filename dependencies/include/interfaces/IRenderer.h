#ifndef _IRENDERER_H__
#define _IRENDERER_H__

#include <unordered_map>
#include <iostream>

#include "IShader.h"
#include "IWindow.h"

class IRenderer
{
public:    
    virtual IShader* GetShader(const std::string& shaderName) = 0;
    virtual void UseShader(const std::string& shaderName) = 0;

    virtual void AttachShader(std::string vPath, std::string fPath, std::string shaderName) = 0;

    virtual IWindow* GetWindow() = 0;
    virtual bool GetWindowCloseStatus() = 0;
    virtual float* GetDeltaTime() = 0;

    virtual void ClearBuffer(uint32_t buffers) = 0;
    virtual void Update() = 0;

    virtual ~IRenderer() = default;
};

#endif 