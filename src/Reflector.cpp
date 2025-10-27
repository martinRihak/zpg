#include "Reflector.hpp"

Reflector::Reflector(glm::vec3 pos, glm::vec3 diff, glm::vec3 att, glm::vec3 dir, float cutOff, float outterCutOff)
: Light(pos,diff,att), direction(dir)
{
    this->cutOff = glm::cos(glm::radians(cutOff));
    this->outterCutOff = glm::cos(glm::radians(outterCutOff));
}
Reflector::~Reflector()
{
}

void Reflector::update(float dt)
{
    notifyAll();
}
glm::vec3 Reflector::getDirection() const{return this->direction;}
float Reflector::getCutOff() const{return this->cutOff;}
float Reflector::getOutterCutOff() const{return this->outterCutOff;}
