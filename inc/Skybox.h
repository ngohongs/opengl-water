#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "stb/stb_image.h"
#include "Geometry.h"
#include "Cube.h"
#include "Shader.h"


class Skybox
{
public:
	Skybox() = default;
	Skybox(const char** faces);
	void Draw(const glm::mat4& proj, const glm::mat4& view);
	GLuint GetTexture() const;
	void Bind(const int& unit);
private:
	GLuint m_Cubemap;
	Geometry m_Cube;
	Shader m_Shader;
};

