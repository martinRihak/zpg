#pragma once

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <assimp/material.h>


class Material
{
private:
    glm::vec3 ambient;  // Kbool hasUva
    glm::vec3 diffuse;  // Kd
    glm::vec3 specular; // Ks
    glm::vec3 objectColor;
    float shininess; // Ns (exponent)
    glm::vec3 emission;

    bool texture ;
    GLuint textureID;
    bool isSkyBox = false;
    std::vector<std::string> faces;
    float time = 1.0f;
public:
    Material();
    Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness);
    ~Material();

    const glm::vec3 &getAmbient() const { return ambient; }
    const glm::vec3 &getDiffuse() const { return diffuse; }
    const glm::vec3 &getSpecular() const { return specular; }
    const glm::vec3 &getObjectColor() const { return objectColor; }
    const glm::vec3 &getEmission() const { return emission; }
    float getShininess() const { return shininess; }

    void setAmbient(const glm::vec3 &a) { ambient = a; }
    void setDiffuse(const glm::vec3 &d) { diffuse = d; }
    void setSpecular(const glm::vec3 &s) { specular = s; }
    void setObjectColor(const glm::vec3 &c) { objectColor = c; }
    void setEmission(const glm::vec3 &e) { emission = e; }
    void setShininess(float s) { shininess = s; }
    void setFaces(const std::vector<std::string>& f) { faces = f; }
    std::vector<std::string>& getFaces() { return faces; }

    void loadTexture(const char* name);
    void loadCubeMap();

    bool isSkyBoxMaterial() const { return isSkyBox; }
    void setSkyBox(bool isSkyBox) { this->isSkyBox = isSkyBox; }

    bool hasTexture() const { return texture; }
    void setTexture(bool texture) { this->texture = texture; }
    GLuint getTextureID()  { return textureID; }
    void setTime(float t) { time = t; }
    float getTime() { return time; }

};