#pragma once

#include <iostream>
#include <string>

#include "GL/glew.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Geometry.h"
#include "Material.h"
#include "Texture.h"


class Model
{
public:
	Model(const std::string& path);
	Model(const Geometry& geometry, const Material& material);
	void Draw(const Shader& shader);
	void DrawNoColor(const Shader& shader);
	void SetScale(const glm::vec3& scale);
	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;
private:
	Geometry m_Geometry;
	Material m_Material;
	Texture m_Texture;
	glm::vec3 m_Position;
};

