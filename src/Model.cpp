#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"  // Nahrazeno Assimp za tinyobjloader
#include <stdexcept>
#include <vector>
#include <limits>  // Pro numeric_limits
#include <iostream>

void Model::Mesh::createVAO()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}
void Model::Mesh::createVBO(const float *vertices, size_t size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void Model::Mesh::createEBO(const unsigned int *indices, size_t size)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void Model::Mesh::configAttributes(int stride, int posOffset, int normOffset, int uvOffset)
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(posOffset));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(normOffset));
    if (uvOffset >= 0)
    { 
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(uvOffset));
    }
}

Model::Model(const float *model, size_t size, int vertexCount)
{
    Mesh* mesh = new Mesh();

    // --- NOVÉ: Výpočet bounds z vertexů ---
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
    for (size_t i = 0; i < vertexCount * 6; i += 6) {  // Předpoklad: pozice + normála (bez UV)
        glm::vec3 pos(model[i], model[i+1], model[i+2]);
        minBounds = glm::min(minBounds, pos);
        maxBounds = glm::max(maxBounds, pos);
    }

    mesh->createVBO(model, size);                                        
    mesh->createVAO();                                                  
    mesh->configAttributes(6 * sizeof(float), 0, 3 * sizeof(float), -1); 
    mesh->indexCount = 0;
    mesh->vertexCount = vertexCount;
    meshes.push_back(mesh);
    glBindVertexArray(0); 
}

Model::Model(const float *model, size_t size, int vertexCount, bool hasUv)
{
    Mesh* mesh = new Mesh();

    // --- NOVÉ: Výpočet bounds z vertexů ---
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
    int stride = hasUv ? 8 : 6;
    for (size_t i = 0; i < vertexCount * stride; i += stride) {
        glm::vec3 pos(model[i], model[i+1], model[i+2]);
        minBounds = glm::min(minBounds, pos);
        maxBounds = glm::max(maxBounds, pos);
    }

    mesh->createVBO(model, size);
    mesh->createVAO();
    int glStride = hasUv ? 8 * sizeof(float) : 6 * sizeof(float);
    int uvOffset = hasUv ? 6 * sizeof(float) : -1;
    mesh->configAttributes(glStride, 0, 3 * sizeof(float), uvOffset);
    mesh->vertexCount = vertexCount;
    mesh->indexCount = 0;
    meshes.push_back(mesh);

    glBindVertexArray(0);
}

Model::Model(const char *name)
{
    tinyobj::ObjReaderConfig reader_config;
   reader_config.mtl_search_path = "";  // Cesta k MTL souborům (uprav podle potřeby)

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(name, reader_config)) {
        std::cerr << "TinyObjLoader error: " << reader.Error() << std::endl;
        std::cout << "Failed to load model: " << name << std::endl;
        return;  // Nebo vyhoď výjimku
    }

    if (!reader.Warning().empty()) {
        std::cerr << "TinyObjLoader warning: " << reader.Warning() << std::endl;
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Inicializace globálních bounds
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& shape : shapes) {
        Mesh* mesh = new Mesh();

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        bool hasNormals = !attrib.normals.empty();
        bool hasUVs = !attrib.texcoords.empty();

        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            size_t fv = shape.mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                // Pozice (povinné)
                float x = attrib.vertices[3 * idx.vertex_index + 0];
                float y = attrib.vertices[3 * idx.vertex_index + 1];
                float z = attrib.vertices[3 * idx.vertex_index + 2];
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // Update bounds
                glm::vec3 pos(x, y, z);
                minBounds = glm::min(minBounds, pos);
                maxBounds = glm::max(maxBounds, pos);

                // Normály
                if (idx.normal_index >= 0 && hasNormals) {
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                // Texturovací koordináty
                if (idx.texcoord_index >= 0 && hasUVs) {
                    vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }
            }
            index_offset += fv;
        }

        // Indices: Pro drawElements – vytvoř 0..n, protože data jsou již triangulovaná
        for (size_t i = 0; i < vertices.size() / (hasUVs ? 8 : 6); ++i) {
            indices.push_back(static_cast<unsigned int>(i));
        }

        mesh->createVBO(vertices.data(), vertices.size() * sizeof(float));
        mesh->createVAO();
        mesh->createEBO(indices.data(), indices.size() * sizeof(unsigned int));

        int stride = (hasUVs ? 8 : 6) * sizeof(float);
        int uvOffset = (hasUVs ? 6 * sizeof(float) : -1);
        mesh->configAttributes(stride, 0, 3 * sizeof(float), uvOffset);

        mesh->indexCount = indices.size();
        mesh->vertexCount = vertices.size() / (hasUVs ? 8 : 6);

        glBindVertexArray(0);

        meshes.push_back(mesh);

        // Volitelně: Textura z materiálu
        if (!shape.mesh.material_ids.empty() && shape.mesh.material_ids[0] >= 0) {
            const auto& mat = materials[shape.mesh.material_ids[0]];
            if (!mat.diffuse_texname.empty()) {
                mesh->texturePath = mat.diffuse_texname;
            }
        }
    }
}

Model::~Model()
{
    meshes.clear();
}

void Model::draw()
{
    for (const Mesh *mesh : meshes)
    {
        glBindVertexArray(mesh->VAO);
        if (mesh->indexCount > 0)
        {
            glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
        }
        glBindVertexArray(0);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cerr << "OpenGL error: " << err << std::endl;
    }
}
const std::string &Model::getTexturePath(size_t meshIndex) const
{
    if (meshIndex < meshes.size())
    {
        return meshes[meshIndex]->texturePath;
    }
    static std::string empty = "";
    return empty;
}