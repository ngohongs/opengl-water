#pragma once
#include "GL/glew.h"

class Cubemap
{
public:
	Cubemap(const int& res);
	GLuint GetTexture() const;
private:
	GLuint m_Cubemap;
};

