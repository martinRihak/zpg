#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>

class Shader
{
private:
    GLuint shader;

public:
    Shader();
    Shader(const char *shaderText, GLenum shaderType);

    void createShader(GLenum shaderType, const char *shaderCode);
    void createShaderFromFile(GLenum shaderType, const char *shaderFile);
    ~Shader();

    GLuint getID() const;
};