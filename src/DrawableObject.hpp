#pragma once

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/vec3.hpp>   // glm::vec3
#include <glm/vec4.hpp>   // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Transformation/Transformation.hpp"
#include "IAnimator/IAnimator.hpp"
#include "Transformation/ITransformation.hpp"
#include "Material.hpp"
class PointLight;
class Light;
class ShaderProgram;
class DrawableObject
{
private:
    Model *model;
    ShaderProgram *shader;
    Transformation *tranformation;
    Material *material;

    std::unique_ptr<IAnimator> animator;
    bool animated = false;
    CompositeTransformation queuedTransforms;
    bool hasLight = false;
    PointLight *light = nullptr;

    glm::vec3 minBounds, maxBounds;
    bool selected = false;
    uint8_t id;

    void drawSkybox(float dt);
    void drawRegular(float dt, const std::vector<Light*> &lights);
public:
    DrawableObject(Model *model, ShaderProgram *shader);
    ~DrawableObject();
    void draw(float dt);
    void draw(float dt, const std::vector<Light *> &lights);
    Transformation &getTransformation();
    void update(float dt);
    IAnimator *getAnimator() const;
    void setAnimated(bool enabled);
    bool isAnimated() const;
    void createRotation(float speedDegPerSec, glm::vec3 axis, int dir);
    void createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f);
    void createRandomMovement(float speed, float baseInterval);
    void createBetweenPoints(glm::vec3 p1, glm::vec3 p2, float speed);
    
    
    void queueTransform(std::shared_ptr<ITransformation> t);
    void applyQueuedTransforms();
    ShaderProgram *getShaderProgram() const;
    DrawableObject *clone() const;

    bool getHasLight() const;
    PointLight *getLight() const;
    void createLight(glm::vec3 diff, glm::vec3 spec, glm::vec3 att);

    Material *getMaterial() const { return material; }
    void createMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness);
    void setMaterial(Material *mat);

    void computeBounds();
    bool intersectsRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, float &dist) const;
    void setSelected(bool selected) { this->selected = selected; }
    bool isSelected() const { return selected; }

    uint8_t getID() const { return id; }
    void setID(uint8_t id) { this->id = id; }
};