#pragma once

#include "Renderer.h"
#include "Geometry.h"
#include "Quad.h"

class SimulationRenderer :
    public Renderer
{
public:
    SimulationRenderer();
    void Render() override;
    const Texture& Result() override;
private:
    RenderTarget m_HeightField[2];
    int m_Swap;
    Shader m_SimulationShader;
};

