#include "IAnimator.hpp"
#include "../DrawableObject.hpp"
#include <glm/gtx/transform.hpp>
#include <random>    // Pro náhodnost
#include <ctime>     // Pro seedování randomu

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
    glm::vec3 centerPos(centerMatrix[3][0], centerMatrix[3][1], centerMatrix[3][2]);

    float rad = glm::radians(angle);
    glm::vec3 offset(radius * cos(rad), 0.0f, radius * sin(rad));
    glm::vec3 newPos = centerPos + offset;

    t.setPosition(newPos);
}

RandomMovementAnimator::RandomMovementAnimator(float speed, float baseChangeInterval)
    : speed(speed), timeSinceLastChange(0.0f), baseChangeInterval(baseChangeInterval) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));  
        seeded = true;
    }
    randomizeDirection();  
}

void RandomMovementAnimator::randomizeDirection() {
    direction = glm::normalize(glm::vec3(
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f,
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f,
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f
    ));
}

void RandomMovementAnimator::update(Transformation &t, float dt) {
    timeSinceLastChange += dt;
    
    float effectiveInterval = baseChangeInterval + (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.5f;  // ±25% variace
    if (timeSinceLastChange > effectiveInterval) {
        randomizeDirection();
        timeSinceLastChange = 0.0f;
    }
    
    glm::vec3 delta = direction * speed * dt;
    t.setPosition(t.getPosition() + delta);
}