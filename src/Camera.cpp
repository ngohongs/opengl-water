#include "Camera.h"

Camera::Camera()
{
	m_Eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Pitch = 0.0f;
	m_Yaw = -90.0f;
}

Camera::Camera(const glm::vec3& eye, const glm::vec3& dir, const glm::vec3& up, const float& pitch, const float& yaw)
{
	m_Eye = eye;
	m_Direction = dir;
	m_Up = up;
	m_Pitch = pitch;
	m_Yaw = yaw;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Eye, m_Eye + m_Direction, m_Up);
}

void Camera::Slide(const float& c, const DirectionEnum& dir)
{
	glm::vec3 right = glm::normalize(glm::cross(m_Direction, m_Up));
	
	glm::vec3 move = glm::vec3(c);

	switch(dir)
	{
		case DirectionEnum::FRONT:
				move *= m_Direction;
				break;
		case DirectionEnum::BACK:
				move *= -m_Direction;
				break;
		case DirectionEnum::RIGHT:
			move *= right;
			break;
		case DirectionEnum::LEFT:
			move *= -right;
			break;
		default:
			break;
	}

	
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), move);
	m_Eye = translateMatrix * glm::vec4(m_Eye, 1.0f);
}

void Camera::Rotate(double xoffset, double yoffset)
{
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
	if (m_Yaw > -270.0f)
		m_Yaw -= 360.0f;
	if (m_Yaw < -450.0f)
		m_Yaw += 360.0f;
		

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Direction = glm::normalize(direction);
}

void Camera::SetEye(const glm::vec3& eye)
{
	m_Eye = eye;
}

void Camera::SetUp(const glm::vec3& up)
{
	m_Up = up;
}

void Camera::SetDirection(const glm::vec3& dir)
{
	m_Direction = dir;
}

glm::vec3 Camera::GetEye() const
{
	return m_Eye;
}

glm::vec3 Camera::GetUp() const
{
	return m_Up;
}

glm::vec3 Camera::GetDirection() const
{
	return m_Direction;
}




