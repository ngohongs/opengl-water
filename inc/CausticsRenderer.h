#pragma once
#include "Renderer.h"
class CausticsRenderer :
    public Renderer
{
public:
    CausticsRenderer(const int& width, const int& height);
    void Render() override;
    const Texture& Result() override;
    const Texture& ResultRecieverPositions();
private:
    int m_Width;
    int m_Height;

    Shader m_Positions;
    Shader m_Normals;
    Shader m_Caustics;
    Shader m_Lowpass;

    RenderTarget m_ReceiverPositions;
    RenderTarget m_RefractivePositions;
    RenderTarget m_RefractiveNormals;
    RenderTarget m_CausticMap;
    RenderTarget m_Filtered;
};

