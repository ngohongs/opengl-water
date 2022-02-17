#include "Shader.h"

Shader::Shader()
{
	m_ID = glCreateProgram();
	for (int i = 0; i < 3; i++)
		m_Shaders[i] = 0;
}


GLuint Shader::AttachShader(ShaderEnum type, const std::string& path)
{
	const char* source;

	std::ifstream in(path);

	if (in.fail())
	{
		std::cout << "ERROR: unable to open shader file\n";
		return 0;
	}

	std::stringstream content;
	content << in.rdbuf();
	std::string str = content.str();
	source = str.c_str();
	in.close();


	GLuint shader;
	int success;
	char log[512];

	shader = glCreateShader(type);
	
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cout << "ERROR: shader compilation failed\n" << log << std::endl;
		return 0;
	}


	glAttachShader(m_ID, shader);

	switch (type)
	{
	case VERTEX:
		m_Shaders[0] = shader;
		break;
	case GEOMETRY:
		m_Shaders[1] = shader;
		break;
	case FRAGMENT:
		m_Shaders[2] = shader;
		break;
	default:
		break;
	}

	return shader;
}

bool Shader::LinkProgram()
{
	int success;
	char log[512];
	glLinkProgram(m_ID);
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, log);
		std::cout << "ERROR: shader linking failed\n" << log << std::endl;
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_Shaders[i])
		{
			glDeleteShader(m_Shaders[i]);
			m_Shaders[i] = 0;
		}
	}
	
	return true;
}

void Shader::Use()
{
	glUseProgram(m_ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniform1i(location, (int)value);
	if (location == -1)
		std::cout << "ERROR\n";
}

void Shader::SetInt(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniform1i(location, value);
	if (location == -1)
		std::cout << "ERROR\n";
}

void Shader::SetFloat(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniform1f(location, value);
	if (location == -1)
		std::cout << "ERROR\n";
}

void Shader::SetVec2(const std::string& name, glm::vec2 value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniform2fv(location, 1, &value[0]);
	if (location == -1)
		std::cout << "ERROR\n";
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	if (location == -1)
		std::cout << "ERROR\n";
	glUniform3fv(location, 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniform4fv(location, 1, &value[0]);
	if (location == -1)
		std::cout << "ERROR\n";
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	if (location == -1)
		std::cout << "ERROR\n";
}
