#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "Subject.hpp"
#include <memory>
#include "Transformation/Transformation.hpp"
#include "IAnimator/IAnimator.hpp"
#include "DrawableObject.hpp"

class Light : public Subject
{
private:
    Transformation transformation;
    glm::vec3 diff;
    glm::vec3 spec;
    glm::vec3 att;
    std::unique_ptr<IAnimator>animator;
    bool animated = false;

public:
    Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s);
    Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s,glm::vec3 a);
    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &pos);
    glm::vec3 getDiff() const;
    void setDiff(const glm::vec3 &d);
    glm::vec3 getSpec() const;
    void setSpec(const glm::vec3 &s);
    glm::vec3 getAtt() const;
    Transformation &getTransformation();
    SubjectType getSubType() const override { return SubjectType::LIGHT; }

    // Nové metody pro animaci
    void createRotation(float speedDegPerSec, glm::vec3 axis);
    void createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f);
    void createRandomMovement(float speed, float baseInterval);
    void update(float dt);
    void setAnimated(bool enabled);
    bool isAnimated() const;

    ~Light();
};