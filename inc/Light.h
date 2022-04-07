#pragma once
#include "glm/glm.hpp"
#include <glm/ext.hpp>

#include "Shader.h"
struct Light
{
public:
	Light() = default;
	Light(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up,
		  const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe);
	glm::mat4 GetViewMatrix() const;
	glm::vec3 m_Pos;
	glm::vec3 m_Dir; 
	glm::vec3 m_Up;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	void Bind(const Shader & shader);
};

