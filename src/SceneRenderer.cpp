#include "SceneRenderer.h"
#include "..\inc\SceneRenderer.h"

SceneRenderer::SceneRenderer(const int& width, const int& height)
{
    m_Width = width;
    m_Height = height;
    
    m_Reflections = { width, height, COLOR_DEPTH, LINEAR };
    m_Refractions = { width, height, COLOR_DEPTH, LINEAR };
    m_ReflectionsPositions = { width, height, COLOR_RENDERBUFFER, LINEAR };
    m_RefractionsPositions = { width, height, COLOR_RENDERBUFFER, LINEAR };
       
    std::cout << m_Water.AttachShader(VERTEX, "shaders/wave.vert") << std::endl;
    std::cout << m_Water.AttachShader(FRAGMENT, "shaders/wave.frag") << std::endl;
    std::cout << "m_Water " << m_Water.LinkProgram() << std::endl;

    std::cout << m_Positions.AttachShader(VERTEX, "shaders/positions.vert") << std::endl;
    std::cout << m_Positions.AttachShader(FRAGMENT, "shaders/positions.frag") << std::endl;
    std::cout << "m_Positions " << m_Positions.LinkProgram() << std::endl;

    std::cout << m_Reciever.AttachShader(VERTEX, "shaders/reciever.vert") << std::endl;
    std::cout << m_Reciever.AttachShader(FRAGMENT, "shaders/reciever.frag") << std::endl;
    std::cout << "m_Reciever " << m_Reciever.LinkProgram() << std::endl;

    const char* faces[] = {
            "resources/skybox/right.jpg",
            "resources/skybox/left.jpg",
            "resources/skybox/top.jpg",
            "resources/skybox/bottom.jpg",
            "resources/skybox/front.jpg",
            "resources/skybox/back.jpg"
    };

    m_Sky = Skybox(faces);

    state.m_ReflectionsPositions = m_ReflectionsPositions.GetColorTexture();
    state.m_ReflectionsPositions = m_RefractionsPositions.GetColorTexture();
    state.m_Reflections = m_Reflections.GetColorTexture();
    state.m_Refractions = m_Refractions.GetColorTexture();
}

void SceneRenderer::Render()
{

//refractions / reflections
    {
        {
            m_RefractionsPositions.Bind();

            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Positions.Use();
            m_Positions.SetBool("duck", false);
            m_Positions.SetBool("under", true);
            m_Positions.SetMat4("projection", state.m_ProjectionMatrix);
            m_Positions.SetMat4("view", state.m_Camera.GetViewMatrix());
            m_Positions.SetBool("wave", false);
            m_Positions.SetBool("inView", false);

            state.m_Models["terrain"].DrawNoColor(m_Positions);

            state.m_Models["cube"].DrawNoColor(m_Positions);

            m_RefractionsPositions.Unbind();
        }
        {
            m_Refractions.Bind();

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            m_Reciever.Use();
            m_Reciever.SetBool("duck", false);
            m_Reciever.SetBool("under", true);
            m_Reciever.SetFloat("waterHeight", 0.0f);
            m_Reciever.SetVec3("cameraPosition", state.m_Camera.GetEye());
            state.m_Light.Bind(m_Reciever);
            m_Reciever.SetMat4("projection", state.m_ProjectionMatrix);
            m_Reciever.SetMat4("view", state.m_Camera.GetViewMatrix());
            m_Reciever.SetMat4("orthogonal", state.m_OrthogonalMatrix);
            m_Reciever.SetMat4("lightView", state.m_Light.GetViewMatrix());
            state.m_CausticMap.Bind(GL_TEXTURE0);
            state.m_RecieverPositions.Bind(GL_TEXTURE1);

            state.m_Models["terrain"].Draw(m_Reciever);

            state.m_Models["cube"].Draw(m_Reciever);

            m_Sky.Draw(state.m_ProjectionMatrix, state.m_Camera.GetViewMatrix());

            m_Refractions.Unbind();
        }
        {
            m_ReflectionsPositions.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_Positions.Use();
            m_Positions.SetBool("duck", false);
            m_Positions.SetBool("under", false);
            m_Positions.SetMat4("projection", state.m_ProjectionMatrix);
            m_Positions.SetMat4("view", state.m_Camera.GetViewMatrix());
            m_Positions.SetBool("wave", false);
            m_Positions.SetBool("inView", false);

            state.m_Models["terrain"].DrawNoColor(m_Positions);

            state.m_Models["cube"].DrawNoColor(m_Positions);

            m_Positions.SetBool("duck", true);
            m_Positions.SetVec3("duckPosition", state.m_Models["duck"].GetPosition());
            m_Positions.SetFloat("texelSize", 1.0f / state.m_Res);
            state.m_HeightField.Bind(GL_TEXTURE2);
            state.m_Models["duck"].DrawNoColor(m_Positions);

            m_ReflectionsPositions.Unbind();
        }
        {
            m_Reflections.Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            m_Reciever.Use();
            m_Reciever.SetBool("duck", false);
            m_Reciever.SetBool("under", false);
            m_Reciever.SetFloat("waterHeight", 0.0f);
            m_Reciever.SetVec3("cameraPosition", state.m_Camera.GetEye());
            state.m_Light.Bind(m_Reciever);
            m_Reciever.SetMat4("projection", state.m_ProjectionMatrix);
            m_Reciever.SetMat4("view", state.m_Camera.GetViewMatrix());
            m_Reciever.SetMat4("orthogonal", state.m_OrthogonalMatrix);
            m_Reciever.SetMat4("lightView", state.m_Light.GetViewMatrix());
            state.m_CausticMap.Bind(GL_TEXTURE0);
            state.m_RecieverPositions.Bind(GL_TEXTURE1);

            state.m_Models["terrain"].Draw(m_Reciever);

            state.m_Models["cube"].Draw(m_Reciever);

            m_Reciever.SetBool("duck", true);
            m_Reciever.SetVec3("duckPosition", state.m_Models["duck"].GetPosition());
            m_Reciever.SetFloat("texelSize", 1.0f / state.m_Res);
            //heightField[1 - i].GetColorTexture().Bind(GL_TEXTURE2);
            state.m_HeightField.Bind(GL_TEXTURE2);
            state.m_Models["duck"].Draw(m_Reciever);


            m_Sky.Draw(state.m_ProjectionMatrix, state.m_Camera.GetViewMatrix());

            m_Reflections.Unbind();
        }
    }
   
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        m_Reciever.Use();
        m_Reciever.SetBool("duck", false);
        m_Reciever.SetBool("diffuseUsed", false);
        m_Reciever.SetFloat("waterHeight", 0.0f);
        m_Reciever.SetVec3("cameraPosition", state.m_Camera.GetEye());
        state.m_Light.Bind(m_Reciever);
        m_Reciever.SetMat4("projection", state.m_ProjectionMatrix);
        m_Reciever.SetMat4("view", state.m_Camera.GetViewMatrix());
        m_Reciever.SetMat4("orthogonal", state.m_OrthogonalMatrix);
        m_Reciever.SetMat4("lightView", state.m_Light.GetViewMatrix());
        state.m_CausticMap.Bind(GL_TEXTURE0);
        state.m_RecieverPositions.Bind(GL_TEXTURE1);
        state.m_HeightField.Bind(GL_TEXTURE2);
        m_Reciever.SetVec3("duckPosition", state.m_Models["duck"].GetPosition());
        m_Reciever.SetFloat("texelSize", 1.0f / state.m_Res);



        state.m_Models["terrain"].Draw(m_Reciever);

        state.m_Models["cube"].Draw(m_Reciever);

        m_Reciever.SetBool("duck", true);
        state.m_Models["duck"].Draw(m_Reciever);

        m_Sky.Draw(state.m_ProjectionMatrix, state.m_Camera.GetViewMatrix());

        m_Water.Use();
        m_Water.SetMat4("projection", state.m_ProjectionMatrix);
        m_Water.SetMat4("view", state.m_Camera.GetViewMatrix());
        m_Water.SetMat4("model", state.m_Geometry["plane"].GetModelMatrix());
        m_Water.SetVec3("cameraPosition", state.m_Camera.GetEye());
        m_Water.SetFloat("texelSize", 1.0f / state.m_Res);
        state.m_HeightField.Bind(GL_TEXTURE0);
        m_Sky.Bind(GL_TEXTURE1);
        m_RefractionsPositions.GetColorTexture().Bind(GL_TEXTURE2);
        m_Refractions.GetColorTexture().Bind(GL_TEXTURE3);
        m_ReflectionsPositions.GetColorTexture().Bind(GL_TEXTURE4);
        m_Reflections.GetColorTexture().Bind(GL_TEXTURE5);
        m_Reflections.GetDepthTexture().Bind(GL_TEXTURE6);
        state.m_Geometry["plane"].Draw();
}

const Texture& SceneRenderer::Result()
{
    // TODO: insert return statement here
    return m_Refractions.GetColorTexture();
}
