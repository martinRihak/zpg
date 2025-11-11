#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h> // Pro extrakci textury
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
    { // Podmínka pro UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(uvOffset));
    }
}

Model::Model(const float *model, size_t size, int vertexCount)
{
    Mesh mesh;
    mesh.createVBO(model, size);                                        // Bind VBO
    mesh.createVAO();                                                   // Bind VAO
    mesh.configAttributes(6 * sizeof(float), 0, 3 * sizeof(float), -1); // Bez UV
    mesh.indexCount = 0;
    mesh.vertexCount = vertexCount;
    meshes.push_back(mesh);
    glBindVertexArray(0); // Unbind
}

Model::Model(const float *model, size_t size, int vertexCount, bool hasUv)
{
    Mesh mesh;
    mesh.createVBO(model, size);
    mesh.createVAO();
    int stride = hasUv ? 8 * sizeof(float) : 6 * sizeof(float);
    int uvOffset = hasUv ? 6 * sizeof(float) : -1;
    mesh.configAttributes(stride, 0, 3 * sizeof(float), uvOffset);
    mesh.vertexCount = vertexCount;
    mesh.indexCount = 0;
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

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh *aiMesh = scene->mMeshes[i];
        Mesh mesh;

        // Naplň vertex data (std::vector<float> vertices) - podobně jako dřív, ale per mesh
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (unsigned int v = 0; v < aiMesh->mNumVertices; ++v)
        {
            // Pozice
            vertices.push_back(aiMesh->mVertices[v].x);
            vertices.push_back(aiMesh->mVertices[v].y);
            vertices.push_back(aiMesh->mVertices[v].z);

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
        mesh.createVBO(vertices.data(), vertices.size() * sizeof(float));
        mesh.createVAO();
        mesh.createEBO(indices.data(), indices.size() * sizeof(unsigned int));

        // Vypočítej stride podle přítomnosti UV
        int stride = (aiMesh->HasTextureCoords(0) ? 8 : 6) * sizeof(float);
        int uvOffset = (aiMesh->HasTextureCoords(0) ? 6 * sizeof(float) : -1); // -1 znamená bez UV
        mesh.configAttributes(stride, 0, 3 * sizeof(float), uvOffset);

        mesh.indexCount = indices.size();
        mesh.vertexCount = aiMesh->mNumVertices; // Přidej toto
        glBindVertexArray(0);
        // Textura per mesh
        meshes.push_back(mesh);
    }
}

Model::~Model()
{
    meshes.clear();
}

void Model::draw()
{
    for (const Mesh &mesh : meshes)
    {
        glBindVertexArray(mesh.VAO);
        if (mesh.indexCount > 0)
        {
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
            std::cout << "test" << std::endl;
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
            std::cout << meshes.size() << std::endl;

            //            std::cout << mesh.VAO<< std::endl;
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
        return meshes[meshIndex].texturePath;
    }
    static std::string empty = "";
    return empty;
}