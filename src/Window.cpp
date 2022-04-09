#include "Window.h"

Window::Window(const int& width, const int& height)
{
    m_Window = nullptr;
    m_Width = width;
    m_Height = height;
}

int Window::Init()
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_Window = glfwCreateWindow(m_Width, m_Height, "OpenGL Water", glfwGetPrimaryMonitor(), NULL);
    if (!m_Window)
    {
        return 0;
    }
    return 1;
}

GLFWwindow* Window::GetGLFWWindow()
{
    // TODO: insert return statement here
    return m_Window;
}

int Window::GetWidth() const
{
    return m_Width;
}

int Window::GetHeight() const
{
    return m_Height;
}

void Window::SetWidth(const int& width)
{
    m_Width = width;
}

void Window::SetHeight(const int& height)
{
    m_Height = height;
}
