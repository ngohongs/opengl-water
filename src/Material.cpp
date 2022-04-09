#include "Material.h"

Material::Material(const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shi)
{
	m_Ambient = amb;
	m_Diffuse = dif;
	m_Specular = spe;
	m_Shine = shi;
}

void Material::Bind(const Shader& shader)
{
	shader.SetVec3("material.amb", m_Ambient);
	shader.SetVec3("material.dif", m_Diffuse);
	shader.SetVec3("material.spe", m_Specular);
	shader.SetFloat("material.shi", m_Shine);
}
