#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>

class Shader
{
private:
    GLuint shader;
public:
    Shader(const char *shaderText, GLenum shaderType);
    ~Shader();
    GLuint getID() const;
};