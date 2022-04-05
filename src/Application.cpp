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
    
    
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS) {
            state.m_M1Prev = state.m_KeyMap[M1];
            state.m_KeyMap[M1] = true;
        }
        if (action == GLFW_RELEASE) {
            state.m_M1Prev = state.m_KeyMap[M1];
            state.m_KeyMap[M1] = false;
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        state.SetAbort(true);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        //imgui
        glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
        glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        state.m_FirstMouse = true;
        
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        //my app
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);


        
    }
        
    if (state.m_KeyMap[M1]) {
        int width = state.GetWindow().GetWidth();
        int height = state.GetWindow().GetHeight();
        glm::vec2 q;
        if (Application::RayCast((double)width / 2.0, (double)height / 2.0, q))
        {
            glm::vec2 uvDropPos = 0.5f * q + glm::vec2(0.5f);

            if (glm::distance(uvDropPos, state.GetDropPos()) > 0.0 || state.m_M1Prev == false) {
                state.SetDropTest(true);
                state.SetDropPos(uvDropPos);
            }
        }
    }

    state.m_M1Prev = state.m_KeyMap[M1];
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


    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        glfwTerminate();
        printf("ERROR: %s", glewGetErrorString(GlewInitResult));
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::GetIO().FontGlobalScale = 1.5;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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
    float plane2Height = -1.0f;
    plane2.SetPosition(glm::vec3(0.0f, plane2Height, 0.0f));
    plane2.SetScale(glm::vec3(1.0f));
    

    planeVert.clear();
    PlaneGenerator().GenerateGrid(width, height, planeVert);
    Geometry grid;
    grid.LoadGrid(planeVert);

    Geometry cube(cubeVertices, cubeNVertices, cubeTriangles, cubeNTriangles);
    cube.SetScale(glm::vec3(0.30f));
    float cubeHeight = -0.45f;
    cube.SetPosition(glm::vec3(0.0f, cubeHeight, 0.0f));
    


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
    

    RenderTarget refractionColor = { width, height, COLOR_DEPTH, LINEAR };
    RenderTarget reflectionColor = { width, height, COLOR_DEPTH, LINEAR };


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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 sandColor({ .76f, .69f, .5f });
    glm::vec3 pinkColor({ 1.0f, 0.0f, 1.0f });
    



    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    int size = res;
    while (!glfwWindowShouldClose(window))
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        cube.SetPosition(glm::vec3(0.0f, cubeHeight, 0.0f));
        plane2.SetPosition(glm::vec3(0.0f, plane2Height, 0.0f));



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
            waveProcess.SetBool("abort", state.GetAbort());
            waveProcess.SetInt("size", size);
            waveProcess.SetFloat("deltaTime", deltaTime);
            glBindTexture(GL_TEXTURE_2D, dropped.GetColor());
            quad.Draw();
            state.SetDropTest(false);
            state.SetAbort(false);
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
        reciever.SetFloat("waterHeight", 0.0f);
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
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, receiverPositions.GetColor());
        glActiveTexture(GL_TEXTURE0);

        reciever.SetVec3("material.amb", 0.4f * sandColor);
        reciever.SetVec3("material.dif", 0.7f * sandColor);
        reciever.SetVec3("material.spe", .31f * sandColor);
        reciever.SetFloat("material.shi", 10.0f);
        reciever.SetMat4("model", plane2.GetModelMatrix());
        plane2.Draw();

        reciever.SetVec3("material.amb", 0.4f * pinkColor);
        reciever.SetVec3("material.dif", 0.7f * pinkColor);
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
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, refractionColor.GetDepth());
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
            glEnable(GL_CLIP_DISTANCE0);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            reciever.Use();
            reciever.SetBool("under", true);
            reciever.SetFloat("waterHeight", 0.0f);
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
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, receiverPositions.GetColor());
            glActiveTexture(GL_TEXTURE0);

            reciever.SetVec3("material.amb", 0.4f * sandColor);
            reciever.SetVec3("material.dif", 0.7f * sandColor);
            reciever.SetVec3("material.spe", .31f * sandColor);
            reciever.SetFloat("material.shi", 10.0f);
            plane2.Draw();


            reciever.SetMat4("model", cube.GetModelMatrix());
            reciever.SetVec3("material.amb", 0.4f * pinkColor);
            reciever.SetVec3("material.dif", 0.7f * pinkColor);
            reciever.SetVec3("material.spe", .31f * pinkColor);
            reciever.SetFloat("material.shi", 10.0f);
            cube.Draw();

            skybox.Draw(proj, view);
            glDisable(GL_CLIP_DISTANCE0);
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
            glEnable(GL_CLIP_DISTANCE0);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            reciever.Use();
            reciever.SetBool("under", false);
            reciever.SetFloat("waterHeight", 0.0f);
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
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, receiverPositions.GetColor());
            glActiveTexture(GL_TEXTURE0);
            reciever.SetMat4("model", cube.GetModelMatrix());

            reciever.SetVec3("material.amb", 0.4f * pinkColor);
            reciever.SetVec3("material.dif", 0.7f * pinkColor);
            reciever.SetVec3("material.spe", .31f * pinkColor);
            reciever.SetFloat("material.shi", 10.0f);
            cube.Draw();

            glDisable(GL_CLIP_DISTANCE0);
        reflectionColor.Unbind();














        // Debug UI ----------------------------------------------------------------------
        glDisable(GL_DEPTH_TEST);

        displayTexture.Use();
        //BL
        glBindTexture(GL_TEXTURE_2D, reflectionColor.GetColor());
        debugdlQuad.Draw();
        //BR
        glBindTexture(GL_TEXTURE_2D, lowPass.GetColor());
        debugddQuad.Draw();
        //TR
        glBindTexture(GL_TEXTURE_2D, refractionColor.GetColor());
        debugQuad.Draw();
        //TL
        glBindTexture(GL_TEXTURE_2D, receiverPositions.GetColor());
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


        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::SliderFloat("Cube height", &cubeHeight, -3.0f, 3.0f);
            ImGui::SliderFloat("Bed height", &plane2Height, -3.0f, 3.0f);
            //ImGui::SliderInt("Size water", &size, 32, 128);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        glfwSwapBuffers(window);
        glfwPollEvents();

        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << GetGLErrorStr(err) << std::endl;
            break;
        }



        
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
            //std::cout << "intersect: " << "(" << p.x << ", " << p.y << ", " << p.z << ")\n";
            q = glm::vec2(p.x, p.z);
            return true;
        }
    }
    return false;
}
