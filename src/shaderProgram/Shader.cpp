#include "Shader.hpp"
Shader::Shader(const char *shaderFile, GLenum shaderType)
{
    // Loading the contents of a file into a variable
    std::ifstream file(shaderFile);
    if (!file.is_open())
    {
        std::cout << "Unable to open file " << shaderFile << std::endl;
        exit(-1);
    }
    std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char * shaderText = shaderCode.c_str();
    shader = glCreateShader(shaderType);
    // Sets the source code of the shader.
    glShaderSource(shader, 1, &shaderText, NULL);
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
Shader::~Shader()
{
    glDeleteShader(this->shader);
}

GLuint Shader::getID() const
{
    return this->shader;
}