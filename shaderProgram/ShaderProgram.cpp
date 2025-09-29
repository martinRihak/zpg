#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const Shader& vertexShader,const Shader& fragmentShader){
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader.getID());
    glAttachShader(this->shaderProgram, fragmentShader.getID());
    glLinkProgram(this->shaderProgram);
}
void ShaderProgram::use(){
    glUseProgram(this->shaderProgram);
    GLint idModelTransform = glGetUniformLocation(this->shaderProgram, "modelMatrix");

}
ShaderProgram::~ShaderProgram(){
    glDeleteProgram(this->shaderProgram);
}


void ShaderProgram::setModelMatrix(const glm::mat4& modelMatrix) {
    GLuint modelLoc = glGetUniformLocation(this->shaderProgram, "modelMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
}