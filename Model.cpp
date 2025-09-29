#include "Model.hpp"


Model::Model(const float* model,size_t size, int vertexCount ){
    
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size ,model, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);   // generate the VAO
    glBindVertexArray(VAO);       // bind the VAO
    
    glEnableVertexAttribArray(0); // enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    this->vertexCount = vertexCount;
}
Model::~Model(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void Model::draw(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,this->vertexCount);
    glBindVertexArray(0);
}