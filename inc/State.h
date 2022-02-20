#pragma once
#include "Camera.h"
#include "Window.h"


extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void processInput();

class State
{
public:
	State() = default;
	State(const int& width, const int& height);
	int Init();
	Window& GetWindow();
	Camera& GetCamera();
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetOrthogonalMatrix() const;
	void SetDropPos(const glm::vec2& q);
	glm::vec2 GetDropPos() const;
	void SetDropTest(const bool& q);
	bool GetDropTest() const;
	bool GetNormalDisplay() const;
	void ToggleNormalDisplay();
	float GetDeltaTime() const;
	void Update();
private:
	float m_DeltaTime;
	float m_NowTime;
	float m_LastTime;
	glm::vec2 m_DropPos;
	bool m_DropTest;
	Window m_Window;
	Camera m_Camera;
	float m_FOV;
	double m_LastX;
	double m_LastY;
	bool m_FirstMouse;
	bool m_NormalDisplay;
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void processInput();
};

extern State state;
