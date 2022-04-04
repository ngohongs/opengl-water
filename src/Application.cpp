#include "Application.h"
#include <iostream>
const char * GetGLErrorStr(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (state.m_FirstMouse)
    {
        state.m_LastX = xpos;
        state.m_LastY = ypos;
        state.m_FirstMouse = false;
    }

    double xoffset = xpos - state.m_LastX;
    double yoffset = state.m_LastY - ypos;
    state.m_LastX = xpos;
    state.m_LastY = ypos;

    state.m_Camera.Rotate(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    int width = state.GetWindow().GetWidth();
    int height = state.GetWindow().GetHeight();
    glm::vec2 q;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (Application::RayCast((double)width / 2.0, (double)height / 2.0, q))
        {
            glm::vec2 uvDropPos = 0.5f * q + glm::vec2(0.5f);
            state.SetDropTest(true);
            state.SetDropPos(uvDropPos);
        }
    }
}
 
void processInput()
{
    GLFWwindow* window = state.GetWindow().GetGLFWWindow();
    Camera& camera = state.GetCamera();
    const float speed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::FRONT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::BACK);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        state.ToggleNormalDisplay();
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // does nothing TODO remove
    glfwSetCursorPos(window, state.GetWindow().GetWidth() / 2.0, state.GetWindow().GetHeight() / 2.0);
}

Application::Application(const int& width, const int& height)
{
    state = State(width, height);
}

int Application::Init()
{
    /* Initialize the library */
    if (!glfwInit())
        return 0;

    if (!state.Init())
    {
        glfwTerminate();
        return 0;
    }
        

    GLFWwindow* window = state.GetWindow().GetGLFWWindow();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        glfwTerminate();
        printf("ERROR: %s", glewGetErrorString(GlewInitResult));
        return 0;
    }

    return 1;
}

int Application::Run()
{
    if (!Init())
        return -1;

    int width = state.GetWindow().GetWidth();
    int height = state.GetWindow().GetHeight();

    Shader drop;
    std::cout << drop.AttachShader(VERTEX, "shaders/drop.vert") << std::endl;
    std::cout << drop.AttachShader(FRAGMENT, "shaders/drop.frag") << std::endl;
    std::cout << "drop " << drop.LinkProgram() << std::endl;

    Shader debug;
    std::cout << debug.AttachShader(VERTEX, "shaders/debug.vert") << std::endl;
    std::cout << debug.AttachShader(FRAGMENT, "shaders/debug.frag") << std::endl;
    std::cout << "debug " << debug.LinkProgram() << std::endl;

    Shader displayTexture;
    std::cout << displayTexture.AttachShader(VERTEX, "shaders/displaytexture.vert") << std::endl;
    std::cout << displayTexture.AttachShader(FRAGMENT, "shaders/displaytexture.frag") << std::endl;
    std::cout << "displayTexture " << displayTexture.LinkProgram() << std::endl;


    Shader wave;
    std::cout << wave.AttachShader(VERTEX, "shaders/wave.vert") << std::endl;
    std::cout << wave.AttachShader(FRAGMENT, "shaders/wave.frag") << std::endl;
    std::cout << "wave " << wave.LinkProgram() << std::endl;

    Shader waveProcess;
    std::cout << waveProcess.AttachShader(VERTEX, "shaders/waveProcess.vert") << std::endl;
    std::cout << waveProcess.AttachShader(FRAGMENT, "shaders/waveProcess.frag") << std::endl;
    std::cout << "waveProcess " << waveProcess.LinkProgram() << std::endl;

    Shader positions;
    std::cout << positions.AttachShader(VERTEX, "shaders/positions.vert") << std::endl;
    std::cout << positions.AttachShader(FRAGMENT, "shaders/positions.frag") << std::endl;
    std::cout << "positions " << positions.LinkProgram() << std::endl;

    Shader normals;
    std::cout << normals.AttachShader(VERTEX, "shaders/normals.vert") << std::endl;
    std::cout << normals.AttachShader(FRAGMENT, "shaders/normals.frag") << std::endl;
    std::cout << "normals " << normals.LinkProgram() << std::endl;

    Shader caustics;
    std::cout << caustics.AttachShader(VERTEX, "shaders/caustics.vert") << std::endl;
    std::cout << caustics.AttachShader(FRAGMENT, "shaders/caustics.frag") << std::endl;
    std::cout << "caustics " << caustics.LinkProgram() << std::endl;

    Shader reciever;
    std::cout << reciever.AttachShader(VERTEX, "shaders/reciever.vert") << std::endl;
    std::cout << reciever.AttachShader(FRAGMENT, "shaders/reciever.frag") << std::endl;
    std::cout << "reciever " << reciever.LinkProgram() << std::endl;

    Shader depth;
    std::cout << depth.AttachShader(VERTEX, "shaders/depth.vert") << std::endl;
    std::cout << depth.AttachShader(FRAGMENT, "shaders/depth.frag") << std::endl;
    std::cout << "depth " << depth.LinkProgram() << std::endl;

    Shader blur;
    std::cout << blur.AttachShader(VERTEX, "shaders/lowpass.vert") << std::endl;
    std::cout << blur.AttachShader(FRAGMENT, "shaders/lowpass.frag") << std::endl;
    std::cout << "blur " << blur.LinkProgram() << std::endl;

    std::cout << "----" << std::endl;
        
    int res = 64;

    std::vector<Vertex> planeVert;
    std::vector<unsigned int> planeInd;
    PlaneGenerator().Generate(res, planeVert, planeInd);
    Geometry plane(planeVert, planeInd);
    plane.SetScale(glm::vec3(1.0f));

    planeVert.clear();
    planeInd.clear();
    PlaneGenerator().Generate(2, planeVert, planeInd);
    Geometry plane2(planeVert, planeInd);
    plane2.SetPosition(glm::vec3(0.0f, -0.4f, 0.0f));
    plane2.SetScale(glm::vec3(1.0f));

    planeVert.clear();
    PlaneGenerator().GenerateGrid(width, height, planeVert);
    Geometry grid;
    grid.LoadGrid(planeVert);

    Geometry cube(cubeVertices, cubeNVertices, cubeTriangles, cubeNTriangles);
    cube.SetScale(glm::vec3(0.3f));
    cube.SetPosition(glm::vec3(0.0f, 0.15f, 0.0f));
    Geometry quad(quadVertices, quadNVertices, quadTriangles, quadNTriangles);
    Geometry debugQuad(dquadVertices, dquadNVertices, dquadTriangles, dquadNTriangles);
    Geometry debuglQuad(dlquadVertices, dlquadNVertices, dlquadTriangles, dlquadNTriangles);
    Geometry debugdlQuad(ddlquadVertices, ddlquadNVertices, ddlquadTriangles, ddlquadNTriangles);
    Geometry debugddQuad(ddquadVertices, ddquadNVertices, ddquadTriangles, ddquadNTriangles);


    Crosshair crosshair;

    const char* faces[] = {
            "resources/skybox/right.jpg",
            "resources/skybox/left.jpg",
            "resources/skybox/top.jpg",
            "resources/skybox/bottom.jpg",
            "resources/skybox/front.jpg",
            "resources/skybox/back.jpg"
        };


    Skybox skybox(faces);

    Light light(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
        { 0.1f, 0.1f, 0.1f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });

   


    RenderTarget heightField[2] = { {res, res, COLOR, LINEAR}, {res, res, COLOR, LINEAR} };
    RenderTarget dropped = {res, res, COLOR, LINEAR};
    RenderTarget receiverPositions = {width, height, COLOR_RENDERBUFFER, NEAREST};
    RenderTarget refractiveNormals = {width, height, COLOR, NEAREST};
    RenderTarget wavePositions = {width, height, COLOR, NEAREST};
    RenderTarget causticMap = {width, height, COLOR, LINEAR};
    
    RenderTarget depthTex = { width, height, COLOR_RENDERBUFFER, LINEAR };
    RenderTarget refractionColor = { width, height, COLOR_RENDERBUFFER, LINEAR };
    RenderTarget reflectionColor = { width, height, COLOR_RENDERBUFFER, LINEAR };

    RenderTarget refrPos = { width, height, COLOR_RENDERBUFFER, LINEAR };
    RenderTarget reflPos = { width, height, COLOR_RENDERBUFFER, LINEAR };


    RenderTarget lowPass = { width, height, COLOR_RENDERBUFFER, LINEAR };



    Camera& camera = state.GetCamera();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLFWwindow* window = state.GetWindow().GetGLFWWindow();
    
    /* Loop until the user closes the window */
    int i = 0;
    
    

    GLenum err;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CLIP_DISTANCE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 sandColor({ .76f, .69f, .5f });
    glm::vec3 pinkColor({ 1.0f, 0.0f, 1.0f });
    
    while (!glfwWindowShouldClose(window))
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        state.Update();
        float deltaTime = state.GetDeltaTime();
        processInput();
        glm::mat4 proj = state.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();


        //Drop
        dropped.Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            drop.Use();
            drop.SetFloat("texelSize", 1.0f / (float)res);
            drop.SetVec2("dropPos", state.GetDropPos());
            drop.SetBool("drop", state.GetDropTest());
            glBindTexture(GL_TEXTURE_2D, heightField[1 - i].GetColor());
            quad.Draw();
            state.SetDropTest(false);
        dropped.Unbind();


        // Shallow water calc
        heightField[i].Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            waveProcess.Use();
            waveProcess.SetFloat("texelSize", 1.0f / (float)res);
            //waveProcess.SetVec2("dropPos", state.GetDropPos());
            //waveProcess.SetBool("drop", state.GetDropTest());
            waveProcess.SetFloat("deltaTime", deltaTime);
            glBindTexture(GL_TEXTURE_2D, dropped.GetColor());
            quad.Draw();
            state.SetDropTest(false);
        heightField[i].Unbind();

        lowPass.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            blur.Use();
            blur.SetVec2("texelSize", glm::vec2(1.0f/ width, 1.0f / height));
            blur.Use();
            glBindTexture(GL_TEXTURE_2D, causticMap.GetColor());
            quad.Draw();
        lowPass.Unbind();

         

        // Display;
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        reciever.Use();
        reciever.SetVec3("cameraPosition", camera.GetEye());

        //reciever.SetVec3("light.pos", light.GetPosition());
        reciever.SetVec3("light.dir", light.GetDirection());
        reciever.SetVec3("light.amb", light.GetAmbient());
        reciever.SetVec3("light.dif", light.GetDiffuse());
        reciever.SetVec3("light.spe", light.GetSpecular());

        reciever.SetMat4("projection", proj);
        reciever.SetMat4("view", view);
        reciever.SetMat4("orthogonal", state.GetOrthogonalMatrix());
        reciever.SetMat4("lightView", light.GetViewMatrix());
        glBindTexture(GL_TEXTURE_2D, causticMap.GetColor());

        reciever.SetVec3("material.amb", .7f * sandColor);
        reciever.SetVec3("material.dif", 1.0f * sandColor);
        reciever.SetVec3("material.spe", .31f * sandColor);
        reciever.SetFloat("material.shi", 10.0f);
        reciever.SetMat4("model", plane2.GetModelMatrix());
        plane2.Draw();

        reciever.SetVec3("material.amb", .7f * pinkColor);
        reciever.SetVec3("material.dif", 1.0f * pinkColor);
        reciever.SetVec3("material.spe", .31f * pinkColor);
        reciever.SetFloat("material.shi", 10.0f);
        reciever.SetMat4("model", cube.GetModelMatrix());
        cube.Draw();

        

        skybox.Draw(proj, view);

        wave.Use();
        wave.SetMat4("projection", proj);
        wave.SetMat4("view", view);
        wave.SetMat4("model", plane.GetModelMatrix());
        wave.SetVec3("cameraPosition", camera.GetEye());
        wave.SetFloat("texelSize", 1.0f / res);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightField[1 - i].GetColor());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, refrPos.GetColor());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, refractionColor.GetColor());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, reflPos.GetColor());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, reflectionColor.GetColor());
        glActiveTexture(GL_TEXTURE0);

        plane.Draw();
        

        refrPos.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            positions.Use();
            positions.SetMat4("projection", proj);
            positions.SetMat4("view", view);
            positions.SetMat4("model", plane2.GetModelMatrix());
            positions.SetBool("wave", false);
            positions.SetBool("inView", false);
            plane2.Draw();

            positions.SetMat4("model", cube.GetModelMatrix());
            cube.Draw();
        refrPos.Unbind();

        refractionColor.Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            reciever.Use();
            reciever.SetVec3("cameraPosition", camera.GetEye());

            //reciever.SetVec3("light.pos", light.GetPosition());
            reciever.SetVec3("light.dir", light.GetDirection());
            reciever.SetVec3("light.amb", light.GetAmbient());
            reciever.SetVec3("light.dif", light.GetDiffuse());
            reciever.SetVec3("light.spe", light.GetSpecular());

            reciever.SetMat4("projection", proj);
            reciever.SetMat4("view", view);
            reciever.SetMat4("model", plane2.GetModelMatrix());
            reciever.SetMat4("orthogonal", state.GetOrthogonalMatrix());
            reciever.SetMat4("lightView", light.GetViewMatrix());
            glBindTexture(GL_TEXTURE_2D, causticMap.GetColor());

            reciever.SetVec3("material.amb", .7f * sandColor);
            reciever.SetVec3("material.dif", 1.0f * sandColor);
            reciever.SetVec3("material.spe", .31f * sandColor);
            reciever.SetFloat("material.shi", 10.0f);
            plane2.Draw();


            reciever.SetMat4("model", cube.GetModelMatrix());
            reciever.SetVec3("material.amb", .7f * pinkColor);
            reciever.SetVec3("material.dif", 1.0f * pinkColor);
            reciever.SetVec3("material.spe", .31f * pinkColor);
            reciever.SetFloat("material.shi", 10.0f);
            cube.Draw();

            skybox.Draw(proj, view);

        refractionColor.Unbind();

        reflPos.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            positions.Use();
            positions.SetMat4("projection", proj);
            positions.SetMat4("view", view);
            positions.SetBool("wave", false);
            positions.SetBool("inView", false);
            positions.SetMat4("model", cube.GetModelMatrix());
            cube.Draw();
        reflPos.Unbind();

        reflectionColor.Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            reciever.Use();
            reciever.SetVec3("cameraPosition", camera.GetEye());

            //reciever.SetVec3("light.pos", light.GetPosition());
            reciever.SetVec3("light.dir", light.GetDirection());
            reciever.SetVec3("light.amb", light.GetAmbient());
            reciever.SetVec3("light.dif", light.GetDiffuse());
            reciever.SetVec3("light.spe", light.GetSpecular());

            reciever.SetMat4("projection", proj);
            reciever.SetMat4("view", view);
            reciever.SetMat4("orthogonal", state.GetOrthogonalMatrix());
            reciever.SetMat4("lightView", light.GetViewMatrix());
            glBindTexture(GL_TEXTURE_2D, causticMap.GetColor());
            reciever.SetMat4("model", cube.GetModelMatrix());

            reciever.SetVec3("material.amb", .7f * pinkColor);
            reciever.SetVec3("material.dif", 1.0f * pinkColor);
            reciever.SetVec3("material.spe", .31f * pinkColor);
            reciever.SetFloat("material.shi", 10.0f);
            cube.Draw();

            skybox.Draw(proj, view);

        reflectionColor.Unbind();














        // Debug UI ----------------------------------------------------------------------
        glDisable(GL_DEPTH_TEST);
        
        /*glBindTexture(GL_TEXTURE_2D, gaussianTexture[1 - i]);
        quad.Draw();*/
        displayTexture.Use();
        //BL
        glBindTexture(GL_TEXTURE_2D, refrPos.GetColor());
        debugdlQuad.Draw();
        //BR
        glBindTexture(GL_TEXTURE_2D, lowPass.GetColor());
        debugddQuad.Draw();
        //TR
        glBindTexture(GL_TEXTURE_2D, reflPos.GetColor());
        debugQuad.Draw();
        //TL
        glBindTexture(GL_TEXTURE_2D, causticMap.GetColor());
        debuglQuad.Draw();

        crosshair.Draw();
       // --------------------------------------------------------------------------------












        // positions 
        receiverPositions.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            positions.Use();
            positions.SetMat4("projection", state.GetOrthogonalMatrix());
            positions.SetMat4("view", light.GetViewMatrix());
            //positions.SetMat4("view", view);
            positions.SetMat4("model", plane2.GetModelMatrix());
            positions.SetBool("wave", false);
            positions.SetBool("inView", false);
            plane2.Draw();

            positions.SetMat4("model", cube.GetModelMatrix());
            cube.Draw();
        receiverPositions.Unbind();

        // normals
        refractiveNormals.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            normals.Use();
            normals.SetBool("wave", true);
            normals.SetMat4("projection", state.GetOrthogonalMatrix());
            normals.SetMat4("view", light.GetViewMatrix());
            normals.SetMat4("model", plane.GetModelMatrix());
            normals.SetFloat("texelSize", 1.0f / res);
            normals.SetBool("inView", false);
            glBindTexture(GL_TEXTURE_2D, heightField[1 - i].GetColor());
            plane.Draw();
        refractiveNormals.Unbind();

        //wave positions
        wavePositions.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            positions.Use();
            positions.SetMat4("projection", state.GetOrthogonalMatrix());
            positions.SetMat4("view", light.GetViewMatrix());
            positions.SetMat4("model", plane.GetModelMatrix());
            positions.SetBool("wave", true);
            positions.SetBool("inView", false);
            glBindTexture(GL_TEXTURE_2D, heightField[1 - i].GetColor());
            plane.Draw();
        wavePositions.Unbind();

        //caustics
        //glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        causticMap.Bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            caustics.Use();
            caustics.SetMat4("projection", state.GetOrthogonalMatrix());
            caustics.SetMat4("view", light.GetViewMatrix());
            caustics.SetMat4("model", glm::mat4(1.0f));
            caustics.SetVec3("light.dir", light.GetDirection());
            //caustics.SetInt("v", nSamples);
            //caustics.SetVec3("light.pos", glm::vec3(0.0f, 2.0f, 0.0f));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, receiverPositions.GetColor());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, refractiveNormals.GetColor());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, wavePositions.GetColor());
            grid.Draw();
            glActiveTexture(GL_TEXTURE0);
        causticMap.Unbind();



        i = 1 - i;


        glfwSwapBuffers(window);
        glfwPollEvents();

        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << GetGLErrorStr(err) << std::endl;
            break;
        }
    }

    glfwTerminate();
    return 0;
}

bool Application::RayCast(double mouseX, double mouseY, glm::vec2& q)
{
    int width = state.GetWindow().GetWidth();
    int height = state.GetWindow().GetHeight();
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLfloat winZ;
    glReadPixels((GLint)mouseX, (GLint)(height - 1 - mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    glm::vec3 winCoords = glm::vec3(mouseX, height - 1 - mouseY, 1);

    glm::mat4 projection = state.GetProjectionMatrix();
    glm::mat4 view = state.GetCamera().GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 dir = glm::unProject(winCoords, view * model, projection, viewport);
    dir = glm::normalize(dir);
    //std::cout << dir.x << ' ' << dir.y << ' ' << dir.z << std::endl;


    glm::vec3 eye = state.GetCamera().GetEye();
    glm::vec3 planeOrig = glm::vec3(0.0f);
    glm::vec3 planeNorm = glm::vec3(0.0f, 1.0f, 0.0f);
    float distance;
    if (glm::intersectRayPlane(eye, dir, planeOrig, planeNorm, distance))
    {
        glm::vec3 p = eye + distance * dir;
        
        if (-1.0f <= p.x && p.x <= 1.0f &&
            -1.0f <= p.z && p.z <= 1.0f)
        {
            std::cout << "intersect: " << "(" << p.x << ", " << p.y << ", " << p.z << ")\n";
            q = glm::vec2(p.x, p.z);
            return true;
        }
    }
    return false;
}
