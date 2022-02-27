#pragma once

#include "GL/glew.h"
#include "State.h"

enum RTTextureFilter {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};

enum RTType {
	COLOR,
	COLOR_DEPTH,
	COLOR_RENDERBUFFER
};

class RenderTarget
{
public:
	RenderTarget(const int& width, const int& height);
	RenderTarget(const int& width, const int& height, const RTType& type, const RTTextureFilter& filter = RTTextureFilter::NEAREST);
	void AttachColor(RTTextureFilter filter = RTTextureFilter::LINEAR);
	void AttachDepth(RTTextureFilter filter = RTTextureFilter::LINEAR);
	void AttachRenderBuffer();
	GLuint GetColor() const;
	GLuint GetDepth() const;
	GLuint GetFBO() const;
	void Bind();
	void Unbind();
private:
	GLuint m_FBO;
	int m_Width;
	int m_Height;
	GLuint m_DepthTexture;
	GLuint m_ColorTexture;
	GLuint m_RBO;
};

