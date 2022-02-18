#pragma once
#include "glm/glm.hpp"
#include <glm/ext.hpp>
class Light
{
public:
	Light(const glm::vec3& pos, const glm::vec3& dir,
		  const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe);
	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;
	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;
	void SetPosition(const glm::vec3& pos);
	void SetDirection(const glm::vec3& dir);
	void SetAmbient(const glm::vec3& amb);
	void SetDiffuse(const glm::vec3& dif);
	void SetSpecular(const glm::vec3& spe);
	glm::mat4 GetViewMatrix() const;
	
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Dir; 
	
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};

