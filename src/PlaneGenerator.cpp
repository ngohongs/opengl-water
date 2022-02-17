#include "PlaneGenerator.h"

void PlaneGenerator::Generate(const int & res, std::vector<Vertex>& vert, std::vector<unsigned int>& ind)
{
    float spacing = 1.0f / (res - 1);
    for (int i = 0; i < res; i++)
    {
        for (int j = 0; j < res; j++)
        {
            Vertex vertex;
            vertex.m_Position = 2.0f * glm::vec3(j * spacing, 0.0f, i * spacing) - glm::vec3(1.0f, 0.0f, 1.0f);
            vertex.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.m_TextureCoords = glm::vec2(j * spacing, i * spacing);
            vert.push_back(vertex);
        }
    }

    for (int i = 0; i < res - 1; ++i)
    {
        for (int j = 0; j < res - 1; ++j)
        {
            unsigned int topLeft = (i * res) + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = ((i + 1) * res) + j;
            unsigned int bottomRight = bottomLeft + 1;
            ind.push_back(topLeft);
            ind.push_back(bottomLeft);
            ind.push_back(topRight);
            ind.push_back(topRight);
            ind.push_back(bottomLeft);
            ind.push_back(bottomRight);
        }
    }
}
