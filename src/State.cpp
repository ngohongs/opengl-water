#include "State.h"
#include "..\inc\State.h"
#include <iostream>
State state;

State::State(const int& width, const int& height)
{
	m_Window = Window(width, height);
	m_LastX = width / 2.0;
	m_LastY = height / 2.0;
	m_FirstMouse = true;
    m_FOV = 54.0f;
	m_DropPos = glm::vec2(0.5f);
	m_NormalDisplay = false;
	m_NowTime = glfwGetTime();
	m_LastTime = 0;
	m_DeltaTime = 0;
	m_DropTest = false;
	m_Abort = false;
	for (auto& i : m_KeyMap)
		i = false;
}

int State::Init()
{
    return m_Window.Init();
}

Window& State::GetWindow()
{
	return m_Window;
}

Camera& State::GetCamera()
{
	return m_Camera;
}

glm::mat4 State::GetProjectionMatrix() const
{
	float width = m_Window.GetWidth();
	float height = m_Window.GetHeight();
	return glm::perspective(glm::radians(m_FOV), (float) width / (float) height, 0.1f, 100.0f);
}

glm::mat4 State::GetOrthogonalMatrix() const
{
	float width = m_Window.GetWidth();
	float height = m_Window.GetHeight();
	return glm::ortho(-3.0f , 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);
}

void State::SetDropPos(const glm::vec2& q)
{
	m_DropPos = q;
}

glm::vec2 State::GetDropPos() const
{
	return m_DropPos;
}

void State::SetDropTest(const bool& q)
{
	m_DropTest = q;
}

bool State::GetDropTest() const
{
	return m_DropTest;
}

void State::SetAbort(const bool& q)
{
	m_Abort = q;
}

bool State::GetAbort() const
{
	return m_Abort;
}

bool State::GetNormalDisplay() const
{
	return m_NormalDisplay;
}

void State::ToggleNormalDisplay()
{
	m_NormalDisplay = !m_NormalDisplay;
}

float State::GetDeltaTime() const
{
	return m_DeltaTime;
}

void State::Update()
{
	m_NowTime = glfwGetTime();
	m_DeltaTime = m_NowTime - m_LastTime;
	m_LastTime = m_NowTime;
}

