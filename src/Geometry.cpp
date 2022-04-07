#include "Geometry.h"

#include <iostream>
Geometry::Geometry()
{
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_VertCnt = 0;
	m_IndCnt = 0;
	m_Pos = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
}

Geometry::Geometry(const std::vector<Vertex>& verts)
{
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_VertCnt = 0;
	m_IndCnt = 0;
	m_Pos = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
	glGenVertexArrays(1, &m_VAO);
	LoadGrid(verts);
}

Geometry::Geometry(const std::vector<Vertex>& verts, const std::vector<unsigned int>& ind)
{
	glGenVertexArrays(1, &m_VAO);
	Load(verts, ind);
}

Geometry::Geometry(const float* verts, const int& vertCnt, const unsigned int * tri, const int& triCnt)
{
	glGenVertexArrays(1, &m_VAO);
	Load(verts, vertCnt, tri, triCnt);
}

void Geometry::Load(const std::vector<Vertex>& verts, const std::vector<unsigned int>& ind)
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TextureCoords));

	m_VertCnt = verts.size();
	m_IndCnt = ind.size();
	m_Pos = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
}

void Geometry::Load(const float* verts, const int& vertCnt, const unsigned int * tri, const int& triCnt)
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertCnt * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCnt * 3 * sizeof(unsigned int), &tri[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TextureCoords));

	m_VertCnt = vertCnt;
	m_IndCnt = triCnt * 3;
	m_Pos = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
}

void Geometry::LoadGrid(const std::vector<Vertex>& verts)
{
	glGenBuffers(1, &m_VBO);
	
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TextureCoords));

	m_VertCnt = verts.size();
	m_Pos = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
}

void Geometry::Bind() const
{
	glBindVertexArray(m_VAO);
}

void Geometry::Draw() const
{
	glBindVertexArray(m_VAO);
	if (m_IndCnt == 0) {
		glDrawArrays(GL_POINTS, 0, m_VertCnt);

	}
	else {
		glDrawElements(GL_TRIANGLES, m_IndCnt, GL_UNSIGNED_INT, 0);
	}
}

void Geometry::SetPosition(const glm::vec3& pos)
{
	m_Pos = pos;
}

void Geometry::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

glm::mat4 Geometry::GetModelMatrix() const
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Pos);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
	glm::mat4 modelMatrix = translationMatrix * scaleMatrix;
	return modelMatrix;
}
