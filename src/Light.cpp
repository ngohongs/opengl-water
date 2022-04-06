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

glm::mat4 Light::GetViewMatrix() const
{
    return glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
}

void Light::Bind(const Shader& shader)
{
    //shader.SetVec3("light.pos", light.GetPosition());
    shader.SetVec3("light.dir", m_Dir);
    shader.SetVec3("light.amb", m_Ambient);
    shader.SetVec3("light.dif", m_Diffuse);
    shader.SetVec3("light.spe", m_Specular);
}
