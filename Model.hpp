#pragma once
#include <GL/glew.h>
#include <iostream>


class Model
{
private:
    GLuint VAO,VBO;
    int vertexCount;
public:
    Model(const float* model,size_t size, int vertexCount);
    ~Model();

    void draw();
};
