#pragma once
// Include GLFW
#include <GL/glew.h>
#include "Shader.hpp"

class ShaderProgram
{
private:
    GLuint shaderProgram;

public:
    ShaderProgram(const Shader& vertexShader,const Shader& fragmentShader);
    ~ShaderProgram();    
    
    bool setShaderProgram(); 
    void use();
    GLuint getProgram();

};

