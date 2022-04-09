#pragma once

#include "GL/glew.h"
#include "Shader.h"

const float crossVert[] = {
        -0.009f, 0.0f,
         0.009f, 0.0f,
         0.0f  , -0.016f,
         0.0f  ,  0.016f,
};


class Crosshair
{
public:
    Crosshair();
    void Draw();
private:
    GLuint m_VAO;
    GLuint m_VBO;
    Shader m_Shader;
};

