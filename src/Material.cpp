#include "Material.hpp"
#include <assimp/scene.h>
#include <SOIL2/SOIL2.h>  // nebo stb_image.h – doporučuji SOIL2
#include <iostream>

Material::Material()
    : ambient(0.1f), diffuse(0.6f), specular(0.8f), shininess(32.0f) {}

Material::~Material() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void Material::loadFromAssimp(const aiMaterial* mtl, const std::string& modelDir) {
    aiColor3D color;

    if (mtl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        ambient = glm::vec3(color.r, color.g, color.b);
    if (mtl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        diffuse = glm::vec3(color.r, color.g, color.b);
    if (mtl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        specular = glm::vec3(color.r, color.g, color.b);

    float shininessVal = 0.0f;
    if (mtl->Get(AI_MATKEY_SHININESS, shininessVal) == AI_SUCCESS)
        shininess = shininessVal;

    // Načti texturu (diffuse)
    aiString texPath;
    if (mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
        std::string fullPath = modelDir + "/" + texPath.C_Str();
        loadTexture(fullPath);
    }
}

void Material::loadTexture(const std::string& path) {
    textureID = loadTextureGL(path);
    hasTexture = (textureID != 0);
}

GLuint Material::loadTextureGL(const std::string& path) {
    int w, h, channels;
    unsigned char* image = SOIL_load_image(path.c_str(), &w, &h, &channels, SOIL_LOAD_AUTO);
    if (!image) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    SOIL_free_image_data(image);
    return texID;
}