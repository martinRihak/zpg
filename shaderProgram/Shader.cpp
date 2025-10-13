#include "Shader.hpp"
Shader::Shader(){
}
Shader::Shader(const char *shaderText, GLenum shaderType)
{
    this->shader = glCreateShader(shaderType);
    glShaderSource(this->shader, 1, &shaderText, NULL);
    glCompileShader(this->shader);

    GLint suss;
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &suss);
    if (!suss)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation failed\n"
                  << infoLog << std::endl;
    }
}
void Shader::createShader(GLenum shaderType, const char *shaderCode)
{
    // Creates an empty shader
    shader = glCreateShader(shaderType);
    // Sets the source code of the shader.
    glShaderSource(shader, 1, &shaderCode, NULL);
    // Compiles the shader source code
    glCompileShader(shader);
    GLint suss;
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &suss);
    if (!suss)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation failed\n"
                  << infoLog << std::endl;
    }
}
void Shader::createShaderFromFile(GLenum shaderType, const char *shaderFile)
{
    // Loading the contents of a file into a variable
    std::ifstream file(shaderFile);
    if (!file.is_open())
    {
        std::cout << "Unable to open file " << shaderFile << std::endl;
        exit(-1);
    }
    std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    createShader(shaderType, shaderCode.c_str());
}
Shader::~Shader()
{
    glDeleteShader(this->shader);
}

GLuint Shader::getID() const
{
    return this->shader;
}