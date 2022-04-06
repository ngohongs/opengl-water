#include "Material.h"
#include "..\inc\Material.h"

void Material::Bind(const Shader& shader)
{
	shader.SetVec3("material.amb", m_Ambient);
	shader.SetVec3("material.dif", m_Diffuse);
	shader.SetVec3("material.spe", m_Specular);
	shader.SetFloat("material.shi", m_Shine);
}
