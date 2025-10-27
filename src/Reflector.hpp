#pragma once 
#include "Light.hpp"

class Reflector : public Light
{
private:
    glm::vec3 direction;
    float cutOff;
    float outterCutOff;
public:
    Reflector(glm::vec3 pos,glm::vec3 diff,glm::vec3 att,glm::vec3 dir,float cutOff,float outterCutOff);
    ~Reflector();


    glm::vec3 getDirection() const;
    float getCutOff() const;
    float getOutterCutOff() const;
    LightType getType() const override {return LightType::REFLECTOR;}
    void update(float dt) override;
};
