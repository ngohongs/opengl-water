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
    GLFWwindow* window = state.m_Window.GetGLFWWindow();
    Camera& camera = state.m_Camera;
    const float speed = 1.0f * state.m_DeltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::FRONT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::BACK);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Slide(speed, DirectionEnum::RIGHT);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        state.m_Abort = true;
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
        int width = state.m_Window.GetWidth();
        int height = state.m_Window.GetHeight();
        glm::vec2 q;
        if (Application::RayCast((double)width / 2.0, (double)height / 2.0, q))
        {
            glm::vec2 uvDropPos = 0.5f * q + glm::vec2(0.5f);

            if (glm::distance(uvDropPos, state.m_DropPos) > 0.0 || state.m_M1Prev == false) {
                state.m_DropTest = true;
                state.m_DropPos = uvDropPos;
            }
        }
    }

    state.m_M1Prev = state.m_KeyMap[M1];
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // does nothing TODO remove
    glfwSetCursorPos(window, state.m_Window.GetWidth() / 2.0, state.m_Window.GetHeight() / 2.0);
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
        

    

    GLFWwindow* window = state.m_Window.GetGLFWWindow();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);   
    glfwSwapInterval(0.5);

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
    ImGui::GetIO().FontGlobalScale = 1.2;



    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    return 1;
}

int Application::Run()
{
    if (!Init())
        return -1;

    int width = state.m_Window.GetWidth();
    int height = state.m_Window.GetHeight();

    Shader displayTexture;
    std::cout << displayTexture.AttachShader(VERTEX, "shaders/displaytexture.vert") << std::endl;
    std::cout << displayTexture.AttachShader(FRAGMENT, "shaders/displaytexture.frag") << std::endl;
    std::cout << "displayTexture " << displayTexture.LinkProgram() << std::endl;


    std::cout << "----" << std::endl;
      
    Crosshair crosshair;
    



    Camera& camera = state.m_Camera;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLFWwindow* window = state.m_Window.GetGLFWWindow();
    
    state.GenerateGeometriesModels();

    
    

    GLenum err;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 cubeColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 color;
    float terrainHeight = 0.0f;
    float cubeHeight = -0.45f;

    SimulationRenderer waterSimulation;
    CausticsRenderer cauticsRenderer(width, height);
    SceneRenderer sceneRenderer(width, height);

    while (!glfwWindowShouldClose(window))
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        state.m_Models["cube"].SetPosition(glm::vec3(0.0f, cubeHeight, 0.0f));
        state.m_Models["terrain"].SetPosition(glm::vec3(0.0f, terrainHeight, 0.0f));


        state.Update();
        processInput();
 


        // sim
        waterSimulation.Render();
        cauticsRenderer.Render();
        sceneRenderer.Render();

      
        // Debug UI ----------------------------------------------------------------------
        {
            //glDisable(GL_DEPTH_TEST);
            //glActiveTexture(GL_TEXTURE0);
            //displayTexture.Use();
            ////BL
            //state.m_RecieverPositions.Bind(GL_TEXTURE7);
            //state.m_Geometry["debugdlQuad"].Draw();
            ////BR
            //state.m_Refractions.Bind(GL_TEXTURE7);
            //state.m_Geometry["debugddQuad"].Draw();
            ////TR
            //state.m_Reflections.Bind(GL_TEXTURE7);
            //state.m_Geometry["debugQuad"].Draw();
            ////TL
            //state.m_HeightField.Bind(GL_TEXTURE7);
            //state.m_Geometry["debuglQuad"].Draw();

            crosshair.Draw();
        }
        // --------------------------------------------------------------------------------

        {
            static float f = 0.0f;
            static int counter = 0;
            int scale = 6;

            float texWidth = state.m_Window.GetWidth() / scale;
            float texHeight = state.m_Window.GetHeight() / scale;
            ImVec2 texSize = ImVec2(texWidth, texHeight);
            const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + main_viewport->Size.x - main_viewport->Size.x / (scale - 1), main_viewport->WorkPos.y), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x / (scale - 1), main_viewport->Size.y), ImGuiCond_Always);

            
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoMove
                         | ImGuiWindowFlags_NoResize
                         | 0;

            ImGui::Begin("Settings", NULL, window_flags);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::ColorEdit3("cube color", (float*)&cubeColor); 
            glm::vec3 color = { cubeColor.x, cubeColor.y, cubeColor.z };
            state.m_Models["cube"].SetMaterial(Material(1.0f * color, 1.0f * color, 1.0f * color, 30.0f));

            ImGui::NewLine();

            if (ImGui::Button("Reset settings")) {
                state.m_Radius = 1.5f;
                state.m_Amplitude = 0.05f;

                state.m_WaveSpeed = 0.2f;
                state.m_WaveDamping = 0.0f;
                state.m_WaveSlope = 0.3f;

                state.m_CausticsAbsorbtion = 1.0f;
                state.m_CausticsPower = 90.0f;

                state.m_FirstGuess = 0.1f;
            }

            ImGui::NewLine();

            ImGui::SliderFloat("Cube height", &cubeHeight, -3.0f, 3.0f);

            ImGui::NewLine();

            ImGui::SliderFloat("Bed height", &terrainHeight, -3.0f, 3.0f);

            if (ImGui::CollapsingHeader("Water simulation settings")) {
                ImGui::Text("Wave parameters:");
                ImGui::SliderFloat("Wave speed", &state.m_WaveSpeed, 0.0f, 1.0f);
                if (state.m_WaveSpeed >= (1.0f / state.m_Res) / state.m_DeltaTime)
                    state.m_WaveSpeed = ((1.0f / state.m_Res) / state.m_DeltaTime) * 0.999f;
                ImGui::SliderFloat("Wave damping", &state.m_WaveDamping, 0.0f, 1.0f);
                ImGui::SliderFloat("Wave slope", &state.m_WaveSlope, 0.0f, 0.5f);

                ImGui::NewLine();

                ImGui::Text("Drop parameters:");
                ImGui::SliderFloat("Drop height", &state.m_Amplitude, 0.0f, 0.5f);
                ImGui::SliderFloat("Drop radius", &state.m_Radius, 0.0f, 5.0f);

                ImGui::NewLine();

                if (ImGui::Button("Reset heightfield"))
                    state.m_Abort = true;

                ImGui::NewLine();

                ImGui::Text("Heightfield water:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t) state.m_HeightField.GetTexture(),
                    ImVec2(texWidth, texWidth));
               
            }

            if (ImGui::CollapsingHeader("Caustics settings")) {
                ImGui::Text("Caustics parameters:");
                ImGui::SliderFloat("Caustics Power", &state.m_CausticsPower, 0.0f, 100.0f);
                ImGui::SliderFloat("Caustics Absorbtion", &state.m_CausticsAbsorbtion, 0.0f, 10.0f);
                
                ImGui::NewLine();


                ImGui::Text("Reciever positions:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_RecieverPositions.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Water normals:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_RefractiveNormals.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Caustics map:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_CausticMap.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Lowpassed caustics map:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Filtered.GetTexture(),
                    texSize
                );
                
               
            }

            if (ImGui::CollapsingHeader("Reflections/refractions settings")) {
                
                ImGui::Text("Reflections/refractions parameters:");
                ImGui::SliderFloat("First guess", &state.m_FirstGuess, 0.0f, 5.0f);

                ImGui::NewLine();

                ImGui::Text("Reflections positions:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_ReflectionsPositions.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Reflections texture:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Reflections.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Refractions positions:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_RefractionsPositions.GetTexture(),
                    texSize
                );

                ImGui::Separator();

                ImGui::Text("Refractions texture:");
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Refractions.GetTexture(),
                    texSize
                );

            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        if (show_demo_window)
            ImGui::ShowDemoWindow();

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
    int width = state.m_Window.GetWidth();
    int height = state.m_Window.GetHeight();
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLfloat winZ;
    glReadPixels((GLint)mouseX, (GLint)(height - 1 - mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    glm::vec3 winCoords = glm::vec3(mouseX, height - 1 - mouseY, 1);



    glm::mat4 projection = state.m_ProjectionMatrix;
    glm::mat4 view = state.m_Camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 dir = glm::unProject(winCoords, view * model, projection, viewport);
    dir = glm::normalize(dir);
    //std::cout << dir.x << ' ' << dir.y << ' ' << dir.z << std::endl;


    glm::vec3 eye = state.m_Camera.GetEye();
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
