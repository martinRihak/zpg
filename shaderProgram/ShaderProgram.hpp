#pragma once
// Include GLFW
#include <GL/glew.h>
#include "Shader.hpp"
#include <memory>
#include <glm/vec3.hpp>                 
#include <glm/vec2.hpp>                 
#include <glm/vec4.hpp>                 
#include <glm/mat4x4.hpp>  
#include "../Camera.hpp"     
#include "../Observer.hpp"        
class Camera;

class ShaderProgram : public Observer
{
private:
    GLuint shaderProgram;
    Camera* camera;
    
public:
    ShaderProgram(const Shader& vertexShader,const Shader& fragmentShader,Camera* camera);
    ~ShaderProgram();    
    
    bool setShaderProgram(); 
    void use();
    void notify();
    void setModelMatrix(const glm::mat4& modelMatrix);
    // Overloaded uniform setters
    void setUniform(const char* name, float value);
    void setUniform(const char* name, int value);
    void setUniform(const char* name, bool value);
    void setUniform(const char* name, const glm::vec2& value);
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::vec4& value);
    void setUniform(const char* name, const glm::mat4& value);
};

