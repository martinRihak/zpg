#include "IAnimator.hpp"
#include "../DrawableObject.hpp"
#include <glm/gtx/transform.hpp>

void NullAnimator::update(Transformation &t, float dt) {

}

RotateAnimator::RotateAnimator(float speedDegPerSec, glm::vec3 axis)
    : speed(speedDegPerSec), axis(axis), angle(0.f) {}

void RotateAnimator::update(Transformation &t, float dt) {
    angle += speed * dt;
    t.setRotation(angle, axis);
}

OrbitAnimator::OrbitAnimator(DrawableObject* center, float radius, float speedDegPerSec, float initialAngleDeg)
    : center(center), radius(radius), speed(speedDegPerSec), angle(initialAngleDeg) {

}

void OrbitAnimator::update(Transformation &t, float dt) {
    angle += speed * dt;

    glm::mat4 centerMatrix = center->getTransformation().getModelMatrix();
    glm::vec3 centerPos(centerMatrix[3]);

    float rad = glm::radians(angle);
    glm::vec3 offset(radius * cos(rad), 0.0f, radius * sin(rad));
    glm::vec3 newPos = centerPos + offset;

    t.setPosition(newPos);
}