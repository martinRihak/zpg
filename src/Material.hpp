#pragma once

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <assimp/material.h>

class Material {
private:
    glm::vec3 ambient;   // Ka
    glm::vec3 diffuse;   // Kd
    glm::vec3 specular;  // Ks
    float shininess;     // Ns (exponent)

    GLuint textureID = 0; // OpenGL texture handle
    bool hasTexture = false;

public:
    Material();
    ~Material();

    // Gettery
    const glm::vec3& getAmbient() const { return ambient; }
    const glm::vec3& getDiffuse() const { return diffuse; }
    const glm::vec3& getSpecular() const { return specular; }
    float getShininess() const { return shininess; }
    GLuint getTextureID() const { return textureID; }
    bool hasTexture() const { return hasTexture; }

    // Settery
    void setAmbient(const glm::vec3& a) { ambient = a; }
    void setDiffuse(const glm::vec3& d) { diffuse = d; }
    void setSpecular(const glm::vec3& s) { specular = s; }
    void setShininess(float s) { shininess = s; }

    // Načtení materiálu z Assimp
    void loadFromAssimp(const aiMaterial* mtl, const std::string& modelDir);

    // Načtení textury (volitelné ruční)
    void loadTexture(const std::string& path);

private:
    GLuint loadTextureGL(const std::string& path);
};