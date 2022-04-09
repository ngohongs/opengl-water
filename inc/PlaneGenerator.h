#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Vertex.h"

class PlaneGenerator
{
public:
	PlaneGenerator() = default;
	void Generate(const int & res, std::vector<Vertex>& vert, std::vector<unsigned int>& ind);
	void GenerateGrid(const int & resX, const int & resY, std::vector<Vertex>& vert);
private:
};

