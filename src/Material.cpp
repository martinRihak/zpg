#include "Material.hpp"
#include <assimp/scene.h>
#include <SOIL/SOIL.h>  // nebo stb_image.h – doporučuji SOIL2
#include <iostream>

Material::Material()
    : ambient(0.1f), diffuse(0.6f), specular(0.1f), shininess(32.0f) , objectColor(1,1,1), emission(0){}

Material::Material(glm::vec3 a,glm::vec3 d,glm::vec3 s,float shiness):ambient(a),diffuse(d),specular(s),shininess(shiness) {}
Material::~Material() {

}