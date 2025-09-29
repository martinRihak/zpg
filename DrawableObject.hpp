#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "shaderProgram/ShaderProgram.hpp"

class DrawableObject
{
private:
    Model* model;
    ShaderProgram* shader;
public:
    DrawableObject(Model* model,ShaderProgram* shader);
    ~DrawableObject();
    void draw();
    Model* getModel() const;
    ShaderProgram* getShaderProgram() const;
};