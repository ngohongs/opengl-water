#include "Crosshair.h"

Crosshair::Crosshair()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crossVert), crossVert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
    m_Shader.AttachShader(VERTEX, "shaders/crosshair.vert");
    m_Shader.AttachShader(FRAGMENT, "shaders/crosshair.frag");
    m_Shader.LinkProgram();
}

void Crosshair::Draw()
{
    m_Shader.Use();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 4);
}
