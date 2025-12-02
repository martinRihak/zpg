#pragma once
// Include GLFW
#include <GL/glew.h>
#include "Shader.hpp"
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "../Camera.hpp"
#include "../Observer.hpp"
#include "../Lights/Light.hpp"
#include "../Material.hpp"
class Camera;
class Light;

class ShaderProgram : public Observer
{
private:
    GLuint shaderProgram;
    void updateCamera(Camera *camera);
    Camera *camera;

    glm::mat4 storedViewMatrix;
    glm::mat4 storedProjMatrix;
    glm::vec3 storedViewPos;
    bool cameraDirty = true;

public:
    ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader, Camera *camera);
    ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader);
    ~ShaderProgram();

    void use();

    void notify(Subject *subject) override;
    void setModelMatrix(const glm::mat4 &modelMatrix);

    void updateLight(int id, Light *light);
    void updateMaterial(Material *mat);

    void setCamera(Camera *camera) { this->camera = camera; }
    Camera *getCamera() const { return this->camera; }

    void setStoredViewMatrix(const glm::mat4 &viewMatrix) { storedViewMatrix = viewMatrix; }


    // Overloaded uniform setters
    void setUniform(const char *name, float value);
    void setUniform(const char *name, int value);
    void setUniform(const char *name, bool value);
    void setUniform(const char *name, const glm::vec2 &value);
    void setUniform(const char *name, const glm::vec3 &value);
    void setUniform(const char *name, const glm::vec4 &value);
    void setUniform(const char *name, const glm::mat4 &value);
};
