#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
class Model
{
private:
    struct Mesh
    {
        GLuint VAO, VBO, EBO;
        size_t indexCount;
        size_t vertexCount;
        std::string texturePath;
        void createVAO();
        void createVBO(const float *vertices, size_t size);
        void createEBO(const unsigned int *indices, size_t size);
        void configAttributes(int stride,int posOffset,int normOffset,int uvOffset);
        Mesh() : VAO(0), VBO(0), EBO(0), indexCount(0) {} 
        ~Mesh()
        { // Destruktor pro čištění bufferů
            glDeleteVertexArrays(1, &VAO);
            std::cout << "Destructor called" << std::endl;
            if (VBO != 0)
            glDeleteBuffers(1, &VBO);
            if (EBO != 0)
                glDeleteBuffers(1, &EBO);
        }
    };
    std::vector<Mesh*> meshes;

public:
    Model(const float *model, size_t size, int vertexCount);
    Model(const char *name);
    Model(const float *model, size_t size, int vertexCount, bool hasUv);

    ~Model();

    void draw();
    size_t getMeshCount() const { return meshes.size(); }
    const std::string &getTexturePath(size_t meshIndex = 0) const;
};
