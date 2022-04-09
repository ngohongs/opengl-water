#include "Application.h"

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        state.m_Stop = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        state.m_KeyMap[E] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
    {
        state.m_KeyMap[E] = false;
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

    if (state.m_KeyMap[E] && !state.m_EPrev) {
        if (state.m_CursorMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);

            state.m_CursorMode = false;
        }
        else {
            glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
            glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            state.m_FirstMouse = true;

            state.m_CursorMode = true;
        }
    }

    state.m_EPrev = state.m_KeyMap[E];
    state.m_M1Prev = state.m_KeyMap[M1];
}

Application::Application(const int& width, const int& height)
{
    state = State(width, height);
}

int Application::Init()
{
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
    glfwSwapInterval(0);

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
    ImGui::GetIO().FontGlobalScale = 1.2f;


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

    GLFWwindow* window = state.m_Window.GetGLFWWindow();
    
    state.GenerateGeometriesModels();
   
    SimulationRenderer waterSimulation;
    CausticsRenderer cauticsRenderer(width, height);
    SceneRenderer sceneRenderer(width, height);

    Crosshair crosshair;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window) && !state.m_Stop)
    {
        state.Update();
        processInput();
 
        waterSimulation.Render();
        cauticsRenderer.Render();
        sceneRenderer.Render();

        crosshair.Draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            int scale = 6;

            float texWidth = (float) (state.m_Window.GetWidth() / scale);
            float texHeight = (float) (state.m_Window.GetHeight() / scale);
            ImVec2 texSize = ImVec2(texWidth, texHeight);
            const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
            float windowWidth = main_viewport->Size.x / (scale - 1);
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + main_viewport->Size.x - main_viewport->Size.x / (scale - 1), main_viewport->WorkPos.y), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, main_viewport->Size.y), ImGuiCond_Always);
            
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("OpenGL Water", NULL, window_flags);     
            ImGui::PushItemWidth(windowWidth / 2.0f);
   
            ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Separator();

            ImGui::TextWrapped("Description:");

            ImGui::Separator();
            ImGui::Separator();

            ImGui::TextWrapped("OpenGL Water is a sandbox displaying heightfield simulation of a water surface proposed by Matthias Fischer-Mueller using graphics API OpenGL. Additionally it has implemeted Shah's caustics formation, custom reflactions and refractions with same intersection algorithm as caustics calculations");

            ImGui::NewLine();

            ImGui::TextWrapped("Apart from the water surface other objects are naively shaded with Phong's empiric model. Color of under water objects are shifted and attuneuated according to the distance from the water surface.");
          
            ImGui::Separator();
            ImGui::Separator();

            ImGui::TextWrapped("Usage:");
            
            ImGui::Separator();
            ImGui::Separator();

            ImGui::TextWrapped("Point crosshair at water surface and press left mouse button to displace it. To adjust parameters of the simulation switch to cursor mode by pressing E, after that you are able to use GUI of the left panel. Press setting headers (e.g. Water simulation settings) to reveal all parameters of the section. To switch back to preview mode press E again.");

            ImGui::Separator();
            ImGui::Separator();

            ImGui::TextWrapped("Controls:");

            ImGui::Separator();
            ImGui::Separator();

            ImGui::Bullet(); ImGui::TextWrapped("to exit program press Esc");
            ImGui::Bullet(); ImGui::TextWrapped("to move press WSAD");
            ImGui::Bullet(); ImGui::TextWrapped("to look around move your mouse");
            ImGui::Bullet(); ImGui::TextWrapped("to displace water press left mouse button");
            ImGui::Bullet(); ImGui::TextWrapped("to toggle cursor press E");
            ImGui::Bullet(); ImGui::TextWrapped("to reset water to rest state press Q");
    
            ImGui::Separator();

            ImGui::NewLine();

            if (ImGui::Button("Reset all settings")) {
                state.Reset();


                state.m_Models["terrain"].SetPosition(glm::vec3(0.0f, state.m_BedHeight, 0.0f));
                state.m_Models["cube"].SetPosition(glm::vec3(0.0f, state.m_CubeHeight, 0.0f));
                state.m_Models["cube"].SetMaterial(Material(state.m_CubeColor, state.m_CubeColor, state.m_CubeColor, 30.0f));
            }

            ImGui::NewLine();

            if (ImGui::CollapsingHeader("Object lighting settings")) {
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Object parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::ColorEdit3("Cube color", (float*)&state.m_CubeColor);
                state.m_Models["cube"].SetMaterial(Material(state.m_CubeColor, state.m_CubeColor, state.m_CubeColor, 30.0f));
                ImGui::SliderFloat("Cube height", &state.m_CubeHeight, -3.0f, 3.0f);
                state.m_Models["cube"].SetPosition(glm::vec3(0.0f, state.m_CubeHeight, 0.0f));

                ImGui::NewLine();

                ImGui::SliderFloat("Bed height", &state.m_BedHeight, -3.0f, 3.0f);
                state.m_Models["terrain"].SetPosition(glm::vec3(0.0f, state.m_BedHeight, 0.0f));
                ImGui::Separator();
                ImGui::TextWrapped("Note: Color can be picked using a color picker by pressing the colored square right of the RGB values.");

                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Light parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SliderFloat("Light absorbtion", &state.m_Attenuation, 0.0f, 5.0f);
                ImGui::NewLine();

                ImGui::SliderFloat("First stage cutoff", &state.m_FirstStage, 0.0f, 3.0f);
                if (state.m_FirstStage >= state.m_SecondStage && state.m_FirstStage + 0.001f <= 3.0f)
                    state.m_SecondStage = state.m_FirstStage + 0.001f;
                ImGui::ColorEdit3("First stage color", (float*)&state.m_FirstStageColor);
                ImGui::NewLine();
                ImGui::SliderFloat("Second stage cutoff", &state.m_SecondStage, 0.0f, 3.0f);
                if (state.m_SecondStage <= state.m_FirstStage && state.m_SecondStage - 0.001f >= 0.0f)
                    state.m_FirstStage = state.m_SecondStage - 0.001f;
                ImGui::ColorEdit3("Second stage color", (float*)&state.m_SecondStageColor);
                ImGui::NewLine();
                ImGui::ColorEdit3("Final stage color", (float*)&state.m_FinalStageColor);
                ImGui::Separator();
                ImGui::TextWrapped("Note: All light properties revolve around the distance of an object from the water surface. Before adjusting these I recommend to play with parameters 'Cube height' and 'Bed height' to observe the effects of the properties above.");
                ImGui::Separator();
                ImGui::NewLine();
            }

            if (ImGui::CollapsingHeader("Water simulation settings")) {
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Wave parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SliderFloat("Wave speed", &state.m_WaveSpeed, 0.0f, 1.0f);
                if (state.m_WaveSpeed >= (1.0f / state.m_Res) / state.m_DeltaTime)
                    state.m_WaveSpeed = ((1.0f / state.m_Res) / state.m_DeltaTime) * 0.999f;
                ImGui::SliderFloat("Wave damping", &state.m_WaveDamping, 0.0f, 1.0f);
                ImGui::SliderFloat("Wave slope", &state.m_WaveSlope, 0.0f, 0.5f);
                ImGui::Separator();
                ImGui::TextWrapped("WARNING: High wave speed can cause instability of the water simulation.");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Drop parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SliderFloat("Drop height", &state.m_Amplitude, 0.0f, 0.5f);
                ImGui::SliderFloat("Drop radius", &state.m_Radius, 0.0f, 5.0f);
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Heightfield water texture:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::NewLine();
                if (ImGui::Button("Reset heightfield"))
                    state.m_Abort = true;
                ImGui::NewLine();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t) state.m_HeightField.GetTexture(),
                    ImVec2(texWidth, texWidth));
                ImGui::Separator();
                ImGui::NewLine();
            }

            if (ImGui::CollapsingHeader("Caustics settings")) {
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Caustics parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SliderFloat("Caustics Power", &state.m_CausticsPower, 0.0f, 100.0f);
                ImGui::SliderFloat("Caustics Absorbtion", &state.m_CausticsAbsorbtion, 0.0f, 5.0f);
                ImGui::Separator();
                ImGui::TextWrapped("Note: Best observed while the water is moving.");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Receiver positions:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_ReceiverPositions.GetTexture(),
                    texSize
                );

                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Water normals:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_RefractiveNormals.GetTexture(),
                    texSize
                );

                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Caustics map:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_CausticMap.GetTexture(),
                    texSize
                );

                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Lowpassed caustics map:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Filtered.GetTexture(),
                    texSize
                );
                ImGui::Separator();
                ImGui::NewLine();
               
            }

            if (ImGui::CollapsingHeader("Reflections/refractions settings")) {
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Reflections/refractions parameters:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SliderFloat("First guess", &state.m_FirstGuess, 0.0f, 5.0f);
                ImGui::Separator();
                ImGui::TextWrapped("Note: Best observed while the water is still.");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Reflections positions:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_ReflectionsPositions.GetTexture(),
                    texSize
                );
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Reflections texture:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Reflections.GetTexture(),
                    texSize
                );
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Refractions positions:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_RefractionsPositions.GetTexture(),
                    texSize
                );
                ImGui::Separator();
                ImGui::Separator();
                ImGui::TextWrapped("Refractions texture:");
                ImGui::Separator();
                ImGui::Separator();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - texWidth) * 0.5f);
                ImGui::Image(
                    (void*)(intptr_t)state.m_Refractions.GetTexture(),
                    texSize
                );
                ImGui::Separator();
                ImGui::TextWrapped("Note: Textures are flipped due to different coordinate systems of texture and screen space.");
                ImGui::Separator();
                ImGui::NewLine();

            }

            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
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
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glm::vec3 winCoords = glm::vec3(mouseX, height - 1 - mouseY, 1);

    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);
    glm::mat4 projection = state.m_ProjectionMatrix;
    glm::mat4 view = state.m_Camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 dir = glm::unProject(winCoords, view * model, projection, viewport);
    dir = glm::normalize(dir);

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
            q = glm::vec2(p.x, p.z);
            return true;
        }
    }
    return false;
}
