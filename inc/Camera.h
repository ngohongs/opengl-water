#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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
	glm::vec3 GetEye() const;
	void Slide(const float& c, const DirectionEnum& dir);
	void Tilt(const float& a);
	void Pan(const float& a);
	void Rotate(double xoffset, double yoffset);
private:
	glm::vec3 m_Direction;
	glm::vec3 m_Eye;
	glm::vec3 m_Up;
	float m_Pitch;
	float m_Yaw;
};

