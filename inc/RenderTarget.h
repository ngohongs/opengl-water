#pragma once

#include "GL/glew.h"

#include "State.h"

#include "Texture.h"

enum RTType {
	COLOR,
	COLOR_DEPTH,
	COLOR_RENDERBUFFER
};

class RenderTarget
{
public:
	RenderTarget();
	RenderTarget(const int& width, const int& height, const RTType & type, const TextureFilter& filter = TextureFilter::LINEAR);
	void AttachColor(TextureFilter filter = TextureFilter::LINEAR);
	void AttachDepth(TextureFilter filter = TextureFilter::LINEAR);
	void AttachRenderBuffer();
	GLuint GetColor() const;
	GLuint GetDepth() const;
	GLuint GetFBO() const;
	const Texture& GetColorTexture() const;
	const Texture& GetDepthTexture() const;
	void Bind();
	void Unbind();
private:
	GLuint m_FBO;

	int m_Width;
	int m_Height;

	GLuint m_DepthTexture;
	GLuint m_ColorTexture;
	GLuint m_RBO;

	Texture m_Color;
	Texture m_Depth;
};

