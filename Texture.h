#pragma once
#include "GL/glew.h"

enum TextureType {
	TEXTURE_2D, 
	CUBE_MAP
};

class Texture
{
public:
	Texture();
private:
	GLuint m_Texture;
	TextureType m_Type;
};

