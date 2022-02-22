#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Vertex.h"
class Geometry
{
public:
	Geometry();
	Geometry(const std::vector<Vertex>& verts, const std::vector<unsigned int> ind);
	Geometry(const float* verts, const int& vertCnt, const unsigned int * tri, const int& triCnt);
	void Load(const std::vector<Vertex>& verts, const std::vector<unsigned int> ind);
	void Load(const float* verts, const int& vertCnt, const unsigned int * tri, const int& indCnt);
	void LoadGrid(const std::vector<Vertex>& verts);
	void Bind() const;
	void Draw() const;
	void SetPosition(const glm::vec3& pos);
	void SetScale(const glm::vec3& scale);
	glm::mat4 GetModelMatrix() const;
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	size_t m_VertCnt;
	size_t m_IndCnt;
	glm::vec3 m_Pos;
	glm::vec3 m_Scale;
};

