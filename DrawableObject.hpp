#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Transformation.hpp"
class DrawableObject
{
private:
    Model* model ;
    ShaderProgram* shader;
    Transformation tranformation;
public:
    DrawableObject(Model* model,ShaderProgram* shader);
    ~DrawableObject();
    void draw();
    Model* getModel() const;
    ShaderProgram* getShaderProgram() const;
    Transformation& getTransformation();
};