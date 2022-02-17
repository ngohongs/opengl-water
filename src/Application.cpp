#include "Application.h"
#include <iostream>


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

    Shader gen;
    std::cout << gen.AttachShader(VERTEX, "shaders/gen.vert") << std::endl;
    std::cout << gen.AttachShader(FRAGMENT, "shaders/gen.frag") << std::endl;
    std::cout << gen.LinkProgram() << std::endl;

    Shader force;
    std::cout << force.AttachShader(VERTEX, "shaders/force.vert") << std::endl;
    std::cout << force.AttachShader(FRAGMENT, "shaders/force.frag") << std::endl;
    std::cout << force.LinkProgram() << std::endl;

    Shader debug;
    std::cout << debug.AttachShader(VERTEX, "shaders/debug.vert") << std::endl;
    std::cout << debug.AttachShader(FRAGMENT, "shaders/debug.frag") << std::endl;
    std::cout << debug.LinkProgram() << std::endl;

    Shader test;
    std::cout << test.AttachShader(VERTEX, "shaders/test.vert") << std::endl;
    std::cout << test.AttachShader(FRAGMENT, "shaders/test.frag") << std::endl;
    std::cout << test.LinkProgram() << std::endl;


    Shader pass;
    std::cout << pass.AttachShader(VERTEX, "shaders/pass.vert") << std::endl;
    std::cout << pass.AttachShader(FRAGMENT, "shaders/pass.frag") << std::endl;
    std::cout << pass.LinkProgram() << std::endl;

    Shader logl;
    std::cout << logl.AttachShader(VERTEX, "shaders/logl.vert") << std::endl;
    std::cout << logl.AttachShader(FRAGMENT, "shaders/logl.frag") << std::endl;
    std::cout << logl.LinkProgram() << std::endl;

    Shader wave;
    std::cout << wave.AttachShader(VERTEX, "shaders/wave.vert") << std::endl;
    std::cout << wave.AttachShader(FRAGMENT, "shaders/wave.frag") << std::endl;
    std::cout << wave.LinkProgram() << std::endl;

    Shader waveProcess;
    std::cout << waveProcess.AttachShader(VERTEX, "shaders/waveProcess.vert") << std::endl;
    std::cout << waveProcess.AttachShader(FRAGMENT, "shaders/waveProcess.frag") << std::endl;
    std::cout << waveProcess.LinkProgram() << std::endl;

    Shader normalDisplay;
    std::cout << normalDisplay.AttachShader(VERTEX, "shaders/normaldisplay.vert") << std::endl;
    std::cout << normalDisplay.AttachShader(GEOMETRY, "shaders/normaldisplay.geom") << std::endl;
    std::cout << normalDisplay.AttachShader(FRAGMENT, "shaders/normaldisplay.frag") << std::endl;
    std::cout << normalDisplay.LinkProgram() << std::endl;


    int res = 64;

    std::vector<Vertex> planeVert;
    std::vector<unsigned int> planeInd;
    PlaneGenerator().Generate(res, planeVert, planeInd);


    Geometry plane(planeVert, planeInd);

    planeVert.clear();
    planeInd.clear();
    PlaneGenerator().Generate(2, planeVert, planeInd);

    Geometry plane2(planeVert, planeInd);
    plane2.SetPosition(glm::vec3(0.0f, -0.4f, 0.0f));
    Geometry cube(cubeVertices, cubeNVertices, cubeTriangles, cubeNTriangles);
    Geometry quad(quadVertices, quadNVertices, quadTriangles, quadNTriangles);
    Geometry debugQuad(dquadVertices, dquadNVertices, dquadTriangles, dquadNTriangles);
    Geometry debuglQuad(dlquadVertices, dlquadNVertices, dlquadTriangles, dlquadNTriangles);


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

    unsigned int framebuffer[2];
    unsigned int textureColorbuffer[2];
    for (int i = 0; i < 2; i++)
    {
        glGenFramebuffers(1, &framebuffer[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]);
        glGenTextures(1, &textureColorbuffer[i]);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, res, res, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLFWwindow* window = state.GetWindow().GetGLFWWindow();
    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */

    int i = 0;

    while (!glfwWindowShouldClose(window))
    {
        processInput();
        

        glm::mat4 proj = state.GetProjectionMatrix();
        glm::mat4 view = state.GetCamera().GetViewMatrix();

        // Shallow water calc
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glViewport(0, 0, res, res);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        waveProcess.Use();
        waveProcess.SetFloat("texelSize", 1.0f / (float)res);
        waveProcess.SetVec2("dropPos", state.GetDropPos());
        waveProcess.SetBool("drop", state.GetDropTest());
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[1 - i]);
        quad.Draw();
        state.SetDropTest(false);

        // Display
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glViewport(0, 0, 1920, 1080);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        wave.Use();
        wave.SetMat4("projection", proj);
        wave.SetMat4("view", view);
        wave.SetMat4("model", plane.GetModelMatrix());
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        plane.Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        gen.Use();
        gen.SetMat4("projection", proj);
        gen.SetMat4("view", view);
        gen.SetMat4("model", plane2.GetModelMatrix());
        plane2.Draw();

        if (state.GetNormalDisplay()) {
            normalDisplay.Use();
            normalDisplay.SetMat4("projection", proj);
            normalDisplay.SetMat4("view", view);
            normalDisplay.SetMat4("model", plane.GetModelMatrix());
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
            plane.Draw();
        }
        


        skybox.Draw(proj, view);




        // UI
        glDisable(GL_DEPTH_TEST);
        debug.Use();
        debug.SetBool("toggle", false);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        debugQuad.Draw();
        debug.SetBool("toggle", true);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        debuglQuad.Draw();

        crosshair.Draw();
       



        i = 1 - i;


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

bool Application::RayCast(double mouseX, double mouseY, glm::vec2& q)
{
    int width = state.GetWindow().GetWidth();
    int height = state.GetWindow().GetHeight();
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);

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
