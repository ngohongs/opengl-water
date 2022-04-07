#include "CausticsRenderer.h"

CausticsRenderer::CausticsRenderer(const int& width, const int& height)
{
    m_Width = width;
    m_Height = height;

    std::cout << m_Positions.AttachShader(VERTEX, "shaders/positions.vert") << std::endl;
    std::cout << m_Positions.AttachShader(FRAGMENT, "shaders/positions.frag") << std::endl;
    std::cout << "m_Positions " << m_Positions.LinkProgram() << std::endl;

    std::cout << m_Normals.AttachShader(VERTEX, "shaders/normals.vert") << std::endl;
    std::cout << m_Normals.AttachShader(FRAGMENT, "shaders/normals.frag") << std::endl;
    std::cout << "m_Normals " << m_Normals.LinkProgram() << std::endl;

    std::cout << m_Caustics.AttachShader(VERTEX, "shaders/caustics.vert") << std::endl;
    std::cout << m_Caustics.AttachShader(FRAGMENT, "shaders/caustics.frag") << std::endl;
    std::cout << "m_Caustics " << m_Caustics.LinkProgram() << std::endl;

    std::cout << m_Lowpass.AttachShader(VERTEX, "shaders/lowpass.vert") << std::endl;
    std::cout << m_Lowpass.AttachShader(FRAGMENT, "shaders/lowpass.frag") << std::endl;
    std::cout << "m_Lowpass " << m_Lowpass.LinkProgram() << std::endl;

    m_ReceiverPositions = { width, height, COLOR_RENDERBUFFER, NEAREST };
    m_RefractiveNormals = { width, height, COLOR, NEAREST };
    m_RefractivePositions = { width, height, COLOR, NEAREST };
    m_CausticMap = { width, height, COLOR, LINEAR };
    m_Filtered = { width, height, COLOR, LINEAR };

    state.m_CausticMap = m_CausticMap.GetColorTexture();
    state.m_RecieverPositions = m_ReceiverPositions.GetColorTexture();
    state.m_Filtered = m_Filtered.GetColorTexture();
    state.m_RefractivePositions = m_ReceiverPositions.GetColorTexture();
    state.m_RefractiveNormals = m_RefractiveNormals.GetColorTexture();
}

void CausticsRenderer::Render()
{
    {
        // positions 
        {
            m_ReceiverPositions.Bind();

            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Positions.Use();

            state.m_HeightField.Bind(GL_TEXTURE0);
            m_Positions.SetFloat("texelSize", 1.0f / state.m_Res);

            m_Positions.SetBool("duck", false);

            m_Positions.SetMat4("projection", state.m_OrthogonalMatrix);
            m_Positions.SetMat4("view", state.m_Light.GetViewMatrix());
            m_Positions.SetBool("wave", false);

            state.m_Models["terrain"].DrawNoColor(m_Positions);
            state.m_Models["cube"].DrawNoColor(m_Positions);

            m_Positions.SetBool("duck", true);
            m_Positions.SetVec3("duckPosition", state.m_Models["duck"].GetPosition());
         
            state.m_Models["duck"].DrawNoColor(m_Positions);
            m_ReceiverPositions.Unbind();
        }

        // normals
        {
            m_RefractiveNormals.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_Normals.Use();
            m_Normals.SetBool("wave", true);
            m_Normals.SetMat4("projection", state.m_OrthogonalMatrix);
            m_Normals.SetMat4("view", state.m_Light.GetViewMatrix());
            m_Normals.SetMat4("model", state.m_Geometry["plane"].GetModelMatrix());
            m_Normals.SetFloat("texelSize", 1.0f / state.m_Res);
            state.m_HeightField.Bind(GL_TEXTURE0);
            state.m_Geometry["plane"].Draw();
            m_RefractiveNormals.Unbind();
        }
        //wave positions
        {
            m_RefractivePositions.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_Positions.Use();
            m_Positions.SetMat4("projection", state.m_OrthogonalMatrix);
            m_Positions.SetMat4("view", state.m_Light.GetViewMatrix());
            m_Positions.SetMat4("model", state.m_Geometry["plane"].GetModelMatrix());
            m_Positions.SetBool("wave", true);
            state.m_HeightField.Bind(GL_TEXTURE0);
            state.m_Geometry["plane"].Draw();
            m_RefractivePositions.Unbind();
        }
        //caustics
        {
            glBlendFunc(GL_ONE, GL_ONE);
            m_CausticMap.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_Caustics.Use();
            m_Caustics.SetMat4("projection", state.m_OrthogonalMatrix);
            m_Caustics.SetMat4("view", state.m_Light.GetViewMatrix());
            m_Caustics.SetMat4("model", glm::mat4(1.0f));
            m_Caustics.SetVec3("light.dir", state.m_Light.m_Dir);
            m_ReceiverPositions.GetColorTexture().Bind(GL_TEXTURE0);
            m_RefractiveNormals.GetColorTexture().Bind(GL_TEXTURE1);
            m_RefractivePositions.GetColorTexture().Bind(GL_TEXTURE2);
            state.m_Geometry["grid"].Draw();
            m_CausticMap.Unbind();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        //lowpass
        {
            m_Filtered.Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 1);


            m_Lowpass.Use();
            m_Lowpass.SetVec2("texelSize", glm::vec2(1.0f / m_Width, 1.0f / m_Height));
            m_Lowpass.Use();
            m_CausticMap.GetColorTexture().Bind(GL_TEXTURE0);
            state.m_Geometry["quad"].Draw();
            m_Filtered.Unbind();
        }
    }
}

const Texture& CausticsRenderer::Result()
{
    return m_Filtered.GetColorTexture();
}

const Texture& CausticsRenderer::ResultRecieverPositions()
{
   return m_ReceiverPositions.GetColorTexture();
}
