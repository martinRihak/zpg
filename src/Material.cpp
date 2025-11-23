#include "Material.hpp"
#include <assimp/scene.h>
#include <SOIL/SOIL.h> // nebo stb_image.h – doporučuji SOIL2
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Material::Material()
    : ambient(0.1f), diffuse(0.6f), specular(0.1f), shininess(32.0f), objectColor(1, 1, 1), emission(0), texture(false) {}

Material::Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness) : ambient(a), diffuse(d), specular(s), shininess(shiness), texture(false) {}
Material::~Material() {}

void Material::loadTexture(const char *name)
{ 
    int text_width, text_height, channels;
    unsigned char *data = stbi_load(name, &text_width, &text_height, &channels, 4);
    printf("%d %d %d\n", text_width, text_height,channels);
    if (!data)
    {
        std::cerr << "Chyba při načítání textury: " << name << " – " << stbi_failure_reason() << std::endl;
        return; 
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    texture = true;
}
void Material::loadCubeMap()
{
}