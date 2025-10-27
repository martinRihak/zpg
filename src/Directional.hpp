#pragma once
#include "Light.hpp"

class Directional : public Light
{
private:
    glm::vec3 direction;

public:
    Directional(glm::vec3 pos, glm::vec3 diff, glm::vec3 att, glm::vec3 dir);
    ~Directional();

    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3 &dir);
    void update(float dt) override;
    LightType getType() const override { return LightType::DIRECTIONAL; }
};