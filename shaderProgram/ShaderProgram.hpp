#pragma once
// Include GLFW
#include <GL/glew.h>
#include "Shader.hpp"

// Include GLM
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4

class ShaderProgram
{
private:
    GLuint shaderProgram;

public:
    ShaderProgram(const Shader& vertexShader,const Shader& fragmentShader);
    ~ShaderProgram();    
    
    bool setShaderProgram(); 
    void use();
    
    void setModelMatrix(const glm::mat4& modelMatrix);
};

