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

RenderTarget::RenderTarget(const int& width, const int& height, const RTType& type, const TextureFilter& filter)
{
    //glGenFramebuffers(1, &m_FBO);
    m_Width = width;
    m_Height = height;
    m_ColorTexture = 0;
    m_DepthTexture = 0;
    m_RBO = 0;

    glGenFramebuffers(1, &m_FBO);

    AttachColor(filter);
    if (type == COLOR_DEPTH) 
        AttachDepth(filter);
    else if (type == COLOR_RENDERBUFFER)
        AttachRenderBuffer();
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void RenderTarget::AttachColor(TextureFilter filter)
{
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    m_Color = Texture(m_Width, m_Height, TextureType::RGB, filter);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Color.GetTexture(), 0);
}

void RenderTarget::AttachDepth(TextureFilter filter)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    m_Depth = Texture(m_Width, m_Height, TextureType::DEPTH, filter);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth.GetTexture(), 0);
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
    return m_Color.GetTexture();
}

GLuint RenderTarget::GetDepth() const
{
    return m_Depth.GetTexture();
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

const Texture& RenderTarget::GetColorTexture() const
{
    return m_Color;
}

const Texture& RenderTarget::GetDepthTexture() const
{
    return m_Depth;
}
