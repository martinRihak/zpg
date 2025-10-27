#include "PointLight.hpp"
PointLight::PointLight(glm::vec3 pos, glm::vec3 d, glm::vec3 s) : Light(pos,d,s,glm::vec3(1.0f, 0.0f, 0.0f)){}
PointLight::PointLight(glm::vec3 pos, glm::vec3 d, glm::vec3 s,glm::vec3 a) : Light(pos,d,s,a){}
void PointLight::createRotation(float speedDegPerSec, glm::vec3 axis)
{
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis);
    this->animated = true;
}

void PointLight::createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg)
{
    this->animator = std::make_unique<OrbitAnimator>(center, radius, speedDegPerSec, initialAngleDeg);
    this->animated = true;
}

void PointLight::createRandomMovement(float speed, float baseInterval){
    this->animator = std::make_unique<RandomMovementAnimator>(speed,baseInterval);
    this->animated = true;
}
void PointLight::update(float dt)
{
    if (animated && animator)
    {
        animator->update(this->getTransformation(), dt);
        notifyAll();
    }
}

void PointLight::setAnimated(bool enabled)
{
    this->animated = enabled;
}

bool PointLight::isAnimated() const
{
    return this->animated;
}
