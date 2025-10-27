#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <vector>

Model::Model(const float *model, size_t size, int vertexCount)
{

    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, model, GL_STATIC_DRAW);

    // Vertex Array Object (VAO)

    glGenVertexArrays(1, &VAO);   // generate the VAO
    glBindVertexArray(VAO);       // bind the VAO
    glEnableVertexAttribArray(0); // enable vertex attributes
    glEnableVertexAttribArray(1); // enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    this->vertexCount = vertexCount;
}
Model::Model(const char *name)
{
    std::string inputfile = name;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(inputfile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        throw std::runtime_error("Failed to load model with Assimp!");
    }

    std::vector<float> vertices;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[i];
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
        {
            aiFace face = mesh->mFaces[f];
            for (unsigned int indx = 0; indx < face.mNumIndices; indx++)
            {
                unsigned int vertexId = face.mIndices[indx];

                vertices.push_back(mesh->mVertices[vertexId].x);
                vertices.push_back(mesh->mVertices[vertexId].y);
                vertices.push_back(mesh->mVertices[vertexId].z);

                // Normály (pokud existují)
                if (mesh->HasNormals())
                {
                    vertices.push_back(mesh->mNormals[vertexId].x);
                    vertices.push_back(mesh->mNormals[vertexId].y);
                    vertices.push_back(mesh->mNormals[vertexId].z);
                }
                else
                {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                // Texturovací koordináty (pokud existují, bereme první set)
                if (mesh->HasTextureCoords(0))
                {
                    vertices.push_back(mesh->mTextureCoords[0][vertexId].x);
                    vertices.push_back(mesh->mTextureCoords[0][vertexId].y);
                }
                else
                {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }
            }
        }
    }

    // Vytvoření VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    this->vertexCount = vertices.size() / 8; // Každý vertex má 8 floats (3 pos + 3 norm + 2 tex)

    // VAO setup (upraveno pro 8 floats: pos + norm + tex)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0); // Pozice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1); // Normály
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2); // Texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
}
Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void Model::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
    glBindVertexArray(0);
}