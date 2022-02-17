#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

enum ShaderEnum {
	VERTEX = GL_VERTEX_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
};

class Shader
{
public:
	Shader();
	GLuint AttachShader(ShaderEnum type, const std::string& path);
	bool LinkProgram();
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, glm::vec2 value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
private:
	GLuint m_ID;
	GLuint m_Shaders[3];
};

