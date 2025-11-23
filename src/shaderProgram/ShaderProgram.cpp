#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Lights/Reflector.hpp"
#include "Lights/Light.hpp"
#include "Lights/Directional.hpp"

ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader, Camera *camera)
    : camera(camera)
{
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader.getID());
    glAttachShader(this->shaderProgram, fragmentShader.getID());
    glLinkProgram(this->shaderProgram);
    //  updateCamera(this->camera);
}

void ShaderProgram::use()
{
    glUseProgram(this->shaderProgram);
    if (cameraDirty)
    {
        GLint viewLoc = glGetUniformLocation(this->shaderProgram, "viewMatrix");
        GLint projLoc = glGetUniformLocation(this->shaderProgram, "projectMatrix");
        if (viewLoc != -1)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &storedViewMatrix[0][0]);
        if (projLoc != -1)
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, &storedProjMatrix[0][0]);
        setUniform("viewPos", storedViewPos);
        cameraDirty = false;
    }
}

void ShaderProgram::notify(Subject *subject)
{
    updateCamera(dynamic_cast<Camera *>(subject));
}

void ShaderProgram::updateCamera(Camera *camera)
{
    if (!camera)
        return;
    storedViewMatrix = camera->getCamera();
    storedProjMatrix = camera->getProjectionMatrix();
    storedViewPos = camera->getPosition();
    cameraDirty = true;
}

void ShaderProgram::updateLight(int index, Light *light)
{
    if (!light || index < 0 || index >= 8)
        return;
    std::string base = "lights[" + std::to_string(index) + "].";
    if (light->getType() == LightType::REFLECTOR)
    {
        Reflector *refl = dynamic_cast<Reflector *>(light);
        if (refl->getIsOn() == false)
        {
            setUniform((base + "diff").c_str(), glm::vec3(0));
            setUniform((base + "spec").c_str(), glm::vec3(0));
            return;
        }
        setUniform((base + "direction").c_str(), refl->getDirection());
        setUniform((base + "cutOff").c_str(), refl->getCutOff());
        setUniform((base + "outterCutOff").c_str(), refl->getOutterCutOff());
        setUniform((base + "lightType").c_str(), 1);
    }

    else if (light->getType() == LightType::DIRECTIONAL)
    {
        Directional *dir = dynamic_cast<Directional *>(light);
        setUniform((base + "direction").c_str(), dir->getDirection());
        setUniform((base + "lightType").c_str(), 2);
    }
    else
    {
        setUniform((base + "lightType").c_str(), 0);
    }
    setUniform((base + "position").c_str(), light->getPosition());
    setUniform((base + "diff").c_str(), light->getDiff());
    setUniform((base + "spec").c_str(), light->getSpec());
    setUniform((base + "constant").c_str(), light->getAtt().x);
    setUniform((base + "linear").c_str(), light->getAtt().y);
    setUniform((base + "quadratic").c_str(), light->getAtt().z);
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
void ShaderProgram::updateMaterial(Material *mat)
{
    setUniform("material.ambient", mat->getAmbient());
    setUniform("material.diffuse", mat->getDiffuse());
    setUniform("material.specular", mat->getSpecular());
    setUniform("material.shininess", mat->getShininess());
    setUniform("material.objectColor", mat->getObjectColor());
    setUniform("material.emission", mat->getEmission());
    setUniform("material.hasTexture", mat->hasTexture());
    setUniform("material.isSkyBox", mat->isSkyBoxMaterial());
    if (mat->hasTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mat->getTextureID());
        this->setUniform("material.ourTexture", 0);
    }
}