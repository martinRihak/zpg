#pragma once 

#include "Light.hpp"


class PointLight : public Light
{
private:
    std::unique_ptr<IAnimator>animator;
    bool animated = false;
public:
    PointLight(glm::vec3 pos, glm::vec3 d, glm::vec3 s);
    PointLight(glm::vec3 pos, glm::vec3 d, glm::vec3 s,glm::vec3 a);

    void createRotation(float speedDegPerSec, glm::vec3 axis,int dir);
    void createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f);
    void createRandomMovement(float speed, float baseInterval);
    void setAnimated(bool enabled);
    bool isAnimated() const;

    void update(float dt) override;
    LightType getType() const override{return LightType::POINTLIGHT;}
};