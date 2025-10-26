#pragma once

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Transformation/Transformation.hpp"
#include "IAnimator/IAnimator.hpp"
#include "Transformation/ITransformation.hpp"
#include "Light.hpp"
class Light;
class ShaderProgram;
class DrawableObject {
private:
    Model* model;
    ShaderProgram* shader;
    Transformation tranformation;
    std::unique_ptr<IAnimator> animator;
    bool animated = false;
    CompositeTransformation queuedTransforms;

public:
    DrawableObject(Model* model, ShaderProgram* shader);
    ~DrawableObject();
    void draw(float dt);
    void draw(float dt,const std::vector<Light*>& lights);
    void createRotation(float speedDegPerSec, glm::vec3 axis); // Opravený název
    void createOrbit(DrawableObject* center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f); 
    Transformation& getTransformation();
    void update(float dt);
    IAnimator* getAnimator() const;
    void setAnimated(bool enabled);
    bool isAnimated() const;
    void queueTransform(std::shared_ptr<ITransformation> t);
    void applyQueuedTransforms();
    ShaderProgram* getShaderProgram() const;
    DrawableObject* clone() const;
};