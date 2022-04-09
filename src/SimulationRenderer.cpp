#include "SimulationRenderer.h"

SimulationRenderer::SimulationRenderer()
{
    m_Swap = 0;

	m_HeightField[0] = RenderTarget{ state.m_Res, state.m_Res, COLOR, LINEAR };
	m_HeightField[1] = RenderTarget{ state.m_Res, state.m_Res, COLOR, LINEAR };

    std::cout << m_SimulationShader.AttachShader(VERTEX, "shaders/waveProcess.vert") << std::endl;
    std::cout << m_SimulationShader.AttachShader(FRAGMENT, "shaders/waveProcess.frag") << std::endl;
    std::cout << "SIM " << m_SimulationShader.LinkProgram() << std::endl;

    state.m_HeightField = m_HeightField[0].GetColorTexture();

}

void SimulationRenderer::Render()
{
    m_HeightField[m_Swap].Bind();

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        m_SimulationShader.Use();

        m_SimulationShader.SetFloat("deltaTime", state.m_DeltaTime);

        m_SimulationShader.SetFloat("texelSize", 1.0f / (float)state.m_Res);

        m_SimulationShader.SetBool("abort", state.m_Abort);

        m_SimulationShader.SetBool("drop", state.m_DropTest);
        m_SimulationShader.SetVec2("dropPos", state.m_DropPos);
        m_SimulationShader.SetFloat("radius", state.m_Radius);
        m_SimulationShader.SetFloat("amplitude", state.m_Amplitude);

        m_SimulationShader.SetFloat("waveSpeed", state.m_WaveSpeed);
        m_SimulationShader.SetFloat("waveDamping", state.m_WaveDamping);
        m_SimulationShader.SetFloat("waveSlope", state.m_WaveSlope);

        m_HeightField[1 - m_Swap].GetColorTexture().Bind(GL_TEXTURE0);

        state.m_Geometry["quad"].Draw();

        state.m_DropTest = false;
        state.m_Abort = false;

    m_HeightField[m_Swap].Unbind();

    m_Swap = 1 - m_Swap;
}

const Texture& SimulationRenderer::Result()
{
    return m_HeightField[0].GetColorTexture();
}
