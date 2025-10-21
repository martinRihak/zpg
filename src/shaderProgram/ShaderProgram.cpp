#include "ShaderProgram.hpp"
#include "Light.hpp"
#include "Camera.hpp"

ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader, Camera *camera) : camera(camera)
{
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader.getID());
    glAttachShader(this->shaderProgram, fragmentShader.getID());
    glLinkProgram(this->shaderProgram);
}

void ShaderProgram::use()
{
    glUseProgram(this->shaderProgram);
    updateCamera(this->camera);
}

void ShaderProgram::notify(Subject *subject)
{
    if (!subject)
        return;
    switch (subject->getSubType())
    {
    case SubjectType::CAMERA:
        updateCamera(dynamic_cast<Camera *>(subject));
        break;
    case SubjectType::LIGHT:
        updateLight(0, dynamic_cast<Light *>(subject)); // Index se nastavuje v DrawableObject
        break;
    }
}

void ShaderProgram::updateCamera(Camera *camera)
{
    if (!camera)
        return;
    GLint viewLoc = glGetUniformLocation(this->shaderProgram, "viewMatrix");
    GLint projLoc = glGetUniformLocation(this->shaderProgram, "projectMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera->getCamera()[0][0]);
    float aspectRatio = 4.0f / 3.0f;
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera->getProjectionMatrix(aspectRatio)[0][0]);
}

void ShaderProgram::updateLight(int index, Light *light)
{
    if (!light || index < 0 || index >= 8)
        return;
    std::string posName = "lights[" + std::to_string(index) + "].position";
    std::string diffName = "lights[" + std::to_string(index) + "].diff";
    std::string specName = "lights[" + std::to_string(index) + "].spec";
    setUniform(posName.c_str(), light->getPosition());
    setUniform(diffName.c_str(), light->getDiff());
    setUniform(specName.c_str(), light->getSpec());
}

glm::vec3 ShaderProgram::getCameraPos()
{
    return camera->getPosition();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(this->shaderProgram);
}

void ShaderProgram::setModelMatrix(const glm::mat4 &modelMatrix)
{
    GLuint modelLoc = glGetUniformLocation(this->shaderProgram, "modelMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
}

void ShaderProgram::setUniform(const char *name, float value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform1f(loc, value);
}

void ShaderProgram::setUniform(const char *name, int value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform1i(loc, value);
}

void ShaderProgram::setUniform(const char *name, bool value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform1i(loc, value ? 1 : 0);
}

void ShaderProgram::setUniform(const char *name, const glm::vec3 &value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform3fv(loc, 1, &value[0]);
}

void ShaderProgram::setUniform(const char *name, const glm::vec2 &value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform2fv(loc, 1, &value[0]);
}

void ShaderProgram::setUniform(const char *name, const glm::vec4 &value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniform4fv(loc, 1, &value[0]);
}

void ShaderProgram::setUniform(const char *name, const glm::mat4 &value)
{
    GLint loc = glGetUniformLocation(this->shaderProgram, name);
    if (loc == -1)
        return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}