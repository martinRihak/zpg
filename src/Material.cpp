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
{ // Přidej parametr pro cestu, abys neměl pevně zakódovaný soubor
    int text_width, text_height, channels;
    stbi_set_flip_vertically_on_load(true); // To je OK, flipuje texturu pro OpenGL (nahoru dolů)
    unsigned char *data = stbi_load(name, &text_width, &text_height, &channels, 4);

    if (!data)
    {
        std::cerr << "Chyba při načítání textury: " << name << " – " << stbi_failure_reason() << std::endl;
        return; // Vrať se, abys nepokračoval s nulovými daty
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Nastavení formátu podle kanálů (RGB nebo RGBA)
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, text_width, text_height, 0, format, GL_UNSIGNED_BYTE, data);

    // Generování mipmap pro lepší kvalitu
    glGenerateMipmap(GL_TEXTURE_2D);

    // Nastavení parametrů textury (opakování, filtr)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Uvolnění dat a unbind textury
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    texture = true;
}
void Material::loadCubeMap()
{
}