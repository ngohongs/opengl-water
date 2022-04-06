#pragma once

#include <iostream>
#include <string>

#include "GL/glew.h"

#include "stb/stb_image.h"

enum TextureFilter {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};

enum TextureType {
	RGB = GL_RGB,
	DEPTH = GL_DEPTH_COMPONENT
};

class Texture
{
public:
	Texture();
	Texture(const int& width, const int& height, const TextureType & type, const TextureFilter& filter);
	Texture(const std::string& path);
	void Bind(const int & unit) const;
	bool Initialized() const;
	GLuint GetTexture() const;
private:
	GLuint m_Texture;
	int m_Width;
	int m_Height;
};

