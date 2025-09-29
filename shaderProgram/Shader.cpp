#include "Shader.hpp"


Shader::Shader(const char* shaderText,GLenum shaderType){
    this->shader = glCreateShader(shaderType); 
    glShaderSource(this->shader,1,&shaderText,NULL);
    glCompileShader(this->shader);

    GLint suss;
    glGetShaderiv(this->shader,GL_COMPILE_STATUS,&suss);
    if(!suss){
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation failed\n" << infoLog << std::endl;
    }
}

Shader::~Shader(){
    glDeleteShader(this->shader);
}

GLuint Shader::getID() const{
    return this->shader;
}