#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
class Window
{
public:
	Window() = default;
	Window(const int& height, const int& width);
	int Init();
	GLFWwindow* GetGLFWWindow();
	int GetWidth() const;
	int GetHeight() const;
	void SetWidth(const int& width);
	void SetHeight(const int& height);
private:
	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
};

