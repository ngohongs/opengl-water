#include "Light.h"

Light::Light(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe)
{
    m_Pos = pos;
    m_Dir = dir;
    m_Up = up;
    m_Ambient = amb;
    m_Diffuse = dif;
    m_Specular = spe;
}

glm::vec3 Light::GetPosition() const
{
    return m_Pos;
}

glm::vec3 Light::GetDirection() const
{
    return m_Dir;
}

glm::vec3 Light::GetAmbient() const
{
    return m_Ambient;
}

glm::vec3 Light::GetDiffuse() const
{
    return m_Diffuse;
}

glm::vec3 Light::GetSpecular() const
{
    return m_Specular;
}

void Light::SetPosition(const glm::vec3& pos)
{
    m_Pos = pos;
}

void Light::SetDirection(const glm::vec3& dir)
{
    m_Dir = dir;
}

void Light::SetAmbient(const glm::vec3& amb)
{
    m_Ambient = amb;
}

void Light::SetDiffuse(const glm::vec3& dif)
{
    m_Diffuse = dif;
}

void Light::SetSpecular(const glm::vec3& spe)
{
    m_Specular = spe;
}

glm::mat4 Light::GetViewMatrix() const
{
    return glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
}
