#pragma once

#include "Renderer.h"
#include "Skybox.h"
#include "Model.h"


#define ALL 7

class SceneRenderer :
    public Renderer
{
public:
    SceneRenderer(const int & width, const int & height);
    void Render() override;
    const Texture& Result() override;
private:
    int m_Width;
    int m_Height;

    Shader m_Positions;

    Shader m_Receiver;
    Shader m_Water;
    
    RenderTarget m_ReflectionsPositions;
    RenderTarget m_RefractionsPositions;
    RenderTarget m_Reflections;
    RenderTarget m_Refractions;

    Skybox m_Sky;
};

