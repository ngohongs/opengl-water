#pragma once

#include "glm/glm.hpp"

#include "Shader.h"

using namespace glm;

struct Material
{
	Material() = default;
	Material(const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shi);
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	float m_Shine;

	void Bind(const Shader& shader);
};

