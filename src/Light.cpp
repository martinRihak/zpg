#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s) : diff(d), spec(s),att(glm::vec3(1.0f, 0.0f, 0.0f))
{
    transformation.setPosition(pos);
}
Light::Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s,glm::vec3 a) : diff(d), spec(s), att(a)
{
    transformation.setPosition(pos);
}

glm::vec3 Light::getPosition() const { return transformation.getPosition(); }

void Light::setPosition(const glm::vec3 &pos)
{
    transformation.setPosition(pos);
    notifyAll();
}

glm::vec3 Light::getDiff() const { return diff; }

void Light::setDiff(const glm::vec3 &d)
{
    diff = d;
    notifyAll();
}

glm::vec3 Light::getSpec() const { return spec; }

void Light::setSpec(const glm::vec3 &s)
{
    spec = s;
    notifyAll();
}
glm::vec3 Light::getAtt() const { return att; }
Transformation &Light::getTransformation()
{
    return transformation;
}

// Nové metody pro animaci
void Light::createRotation(float speedDegPerSec, glm::vec3 axis)
{
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis);
    this->animated = true;
}

void Light::createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg)
{
    this->animator = std::make_unique<OrbitAnimator>(center, radius, speedDegPerSec, initialAngleDeg);
    this->animated = true;
}

void Light::createRandomMovement(float speed, float baseInterval){
    this->animator = std::make_unique<RandomMovementAnimator>(speed,baseInterval);
    this->animated = true;
}
void Light::update(float dt)
{
    if (animated && animator)
    {
        animator->update(transformation, dt);
        notifyAll();
    }
}

void Light::setAnimated(bool enabled)
{
    this->animated = enabled;
}

bool Light::isAnimated() const
{
    return this->animated;
}

Light::~Light() {}