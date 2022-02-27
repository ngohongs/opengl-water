#include "RenderTarget.h"
#include <iostream>

RenderTarget::RenderTarget(const int& width, const int& height)
{
	glGenFramebuffers(1, &m_FBO);
	m_Width = width;
	m_Height = height;
    m_ColorTexture = 0;
    m_DepthTexture = 0;
    m_RBO = 0;
}

RenderTarget::RenderTarget(const int& width, const int& height, const RTType& type, const RTTextureFilter& filter)
{
    //glGenFramebuffers(1, &m_FBO);
    m_Width = width;
    m_Height = height;
    m_ColorTexture = 0;
    m_DepthTexture = 0;
    m_RBO = 0;

    glGenFramebuffers(1, &m_FBO);

    AttachColor();
    if (type == COLOR_DEPTH)
        AttachDepth();
    else if (type == COLOR_RENDERBUFFER)
        AttachRenderBuffer();
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void RenderTarget::AttachColor(RTTextureFilter filter)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glGenTextures(1, &m_ColorTexture);
    glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);
}

void RenderTarget::AttachDepth(RTTextureFilter filter)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH, GL_TEXTURE_2D, m_DepthTexture, 0);
}

void RenderTarget::AttachRenderBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}

GLuint RenderTarget::GetColor() const
{
    return m_ColorTexture;
}

GLuint RenderTarget::GetDepth() const
{
    return m_DepthTexture;
}

void RenderTarget::Bind()
{
    glViewport(0, 0, m_Width, m_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void RenderTarget::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, state.GetWindow().GetWidth(), state.GetWindow().GetHeight());
}

GLuint RenderTarget::GetFBO() const
{
    return m_FBO;
}
