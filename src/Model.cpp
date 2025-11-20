#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h> 
#include <stdexcept>
#include <vector>

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
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        throw std::runtime_error("Failed to load model with Assimp!");
    }

    // --- NOVÉ: Inicializace globálních bounds ---
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh *aiMesh = scene->mMeshes[i];
        Mesh* mesh = new Mesh();

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (unsigned int v = 0; v < aiMesh->mNumVertices; ++v)
        {
            // Pozice
            float x = aiMesh->mVertices[v].x;
            float y = aiMesh->mVertices[v].y;
            float z = aiMesh->mVertices[v].z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // --- NOVÉ: Update bounds z pozic ---
            glm::vec3 pos(x, y, z);
            minBounds = glm::min(minBounds, pos);
            maxBounds = glm::max(maxBounds, pos);

            // Normály
            if (aiMesh->HasNormals())
            {
                vertices.push_back(aiMesh->mNormals[v].x);
                vertices.push_back(aiMesh->mNormals[v].y);
                vertices.push_back(aiMesh->mNormals[v].z);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            // Texturovací koordináty
            if (aiMesh->HasTextureCoords(0))
            {
                vertices.push_back(aiMesh->mTextureCoords[0][v].x);
                vertices.push_back(aiMesh->mTextureCoords[0][v].y);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
        for (unsigned int f = 0; f < aiMesh->mNumFaces; ++f)
        {
            aiFace face = aiMesh->mFaces[f];
            for (unsigned int indx = 0; indx < face.mNumIndices; ++indx)
            {
                indices.push_back(face.mIndices[indx]);
            }
        }
        mesh->createVBO(vertices.data(), vertices.size() * sizeof(float));
        mesh->createVAO();
        mesh->createEBO(indices.data(), indices.size() * sizeof(unsigned int));

        int stride = (aiMesh->HasTextureCoords(0) ? 8 : 6) * sizeof(float);
        int uvOffset = (aiMesh->HasTextureCoords(0) ? 6 * sizeof(float) : -1); // -1 znamená bez UV
        mesh->configAttributes(stride, 0, 3 * sizeof(float), uvOffset);

        mesh->indexCount = indices.size();
        mesh->vertexCount = aiMesh->mNumVertices; 
        glBindVertexArray(0);
       
        meshes.push_back(mesh);
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