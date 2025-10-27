#pragma once 
#include "Light.hpp"

class Reflector : public Light
{
private:
    glm::vec3 direction;
    float cutOff;
    float outterCutOff;
public:
    Reflector(glm::vec3 pos,glm::vec3 diff,glm::vec3 att,glm::vec3 dirr,float cutOff,float outterCutOff);
    ~Reflector();
};
