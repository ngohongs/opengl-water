#include "Skybox.h"

Skybox::Skybox(const char** faces)
{
    glGenTextures(1, &m_Cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

    int width, height, channels;
    for (int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(faces[i], &width, &height, &channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_Cube = Geometry(cubeVertices, cubeNVertices, cubeTriangles, cubeNTriangles);
    
    m_Shader.AttachShader(VERTEX, "shaders/skybox.vert");
    m_Shader.AttachShader(FRAGMENT, "shaders/skybox.frag");
    m_Shader.LinkProgram();
}

void Skybox::Draw(const glm::mat4& proj, const glm::mat4& view)
{
    glDepthFunc(GL_LEQUAL);
    m_Shader.Use();
    m_Shader.SetMat4("projection", proj);
    m_Shader.SetMat4("view", glm::mat4(glm::mat3(view)));
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);
    m_Cube.Bind();
    m_Cube.Draw();
    glDepthFunc(GL_LESS);
}
