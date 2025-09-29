#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const Shader& vertexShader,const Shader& fragmentShader){
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader.getID());
    glAttachShader(this->shaderProgram, fragmentShader.getID());
    glLinkProgram(this->shaderProgram);
}
void ShaderProgram::use(){
    glUseProgram(this->shaderProgram);
}
ShaderProgram::~ShaderProgram(){
    glDeleteProgram(this->shaderProgram);
}
GLuint ShaderProgram::getProgram(){
    return  this->shaderProgram;
}