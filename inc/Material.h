#pragma once

#include "glm/glm.hpp"

#include "Shader.h"

using namespace glm;

struct Material
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	float m_Shine;

	void Bind(const Shader& shader);
};

