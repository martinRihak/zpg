#pragma once

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <assimp/material.h>

class Material
{
private:
    glm::vec3 ambient;  // Ka
    glm::vec3 diffuse;  // Kd
    glm::vec3 specular; // Ks
    glm::vec3 objectColor;
    float shininess; // Ns (exponent)
    glm::vec3 emission;

public:
    Material();
    Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness);
    ~Material();

    const glm::vec3 &getAmbient() const { return ambient; }
    const glm::vec3 &getDiffuse() const { return diffuse; }
    const glm::vec3 &getSpecular() const { return specular; }
    const glm::vec3 &getObjectColor() const { return objectColor; }
    const glm::vec3 &getEmission() const { return emission;}
    float getShininess() const { return shininess; }

    void setAmbient(const glm::vec3 &a) { ambient = a; }
    void setDiffuse(const glm::vec3 &d) { diffuse = d; }
    void setSpecular(const glm::vec3 &s) { specular = s; }
    void setObjectColor(const glm::vec3 &c) { objectColor = c; }
    void setEmission(const glm::vec3 &e) { emission = e; }
    void setShininess(float s) { shininess = s; }
};