#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Material.hpp"
class Model
{
private:
    struct Mesh
    {
        GLuint VAO, VBO, EBO;
        size_t indexCount;
        size_t vertexCount;
        std::string texturePath;
        Material *material;
        void createVAO();
        void createVBO(const float *vertices, size_t size);
        void createEBO(const unsigned int *indices, size_t size);
        void configAttributes(int stride, int posOffset, int normOffset, int uvOffset);

        Mesh() : VAO(0), VBO(0), EBO(0), indexCount(0) {}
        ~Mesh()
        {
            glDeleteVertexArrays(1, &VAO);
            std::cout << "Destructor called" << std::endl;
            if (VBO != 0)
                glDeleteBuffers(1, &VBO);
            if (EBO != 0)
                glDeleteBuffers(1, &EBO);
        }
    };
    std::vector<Mesh *> meshes;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

public:
    Model(const float *model, size_t size, int vertexCount);
    Model(const char *name);
    Model(const float *model, size_t size, int vertexCount, bool hasUv);

    ~Model();

    void drawMesh(size_t index);
    size_t getMeshCount() const { return meshes.size(); }
    const std::string &getTexturePath(size_t meshIndex = 0) const;
    glm::vec3 getMinBounds() const { return minBounds; }
    glm::vec3 getMaxBounds() const { return maxBounds; }
    Material *getMaterial(size_t index) const
    {
        if (index < meshes.size())
            return meshes[index]->material;
        return nullptr;
    }
    void setMaterial(Material *mat) { meshes[0]->material = mat; }
    void freeMaterial() const;;
};