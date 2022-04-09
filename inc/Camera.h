#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

enum class DirectionEnum {
	FRONT,
	BACK,
	RIGHT,
	LEFT
};

class Camera
{
public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& dir, const glm::vec3& up, const float& pitch, const float& yaw);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetFlippedViewMatrix() const;
	void Slide(const float& c, const DirectionEnum& dir);
	void Tilt(const float& a);
	void Pan(const float& a);
	void Rotate(double xoffset, double yoffset);
	void SetEye(const glm::vec3& eye);
	void SetUp(const glm::vec3& up);
	void SetDirection(const glm::vec3& dir);
	glm::vec3 GetEye() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetDirection() const;
private:
	glm::vec3 m_Direction;
	glm::vec3 m_Eye;
	glm::vec3 m_Up;
	float m_Pitch;
	float m_Yaw;
};

