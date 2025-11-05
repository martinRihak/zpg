#include "Reflector.hpp"

Reflector::Reflector(glm::vec3 pos, glm::vec3 diff, glm::vec3 spe, glm::vec3 dir, float cutOff, float outterCutOff)
: Light(pos,diff,spe), direction(dir)
{
    this->cutOff = glm::cos(glm::radians(cutOff));
    this->outterCutOff = glm::cos(glm::radians(outterCutOff));
}
Reflector::Reflector(glm::vec3 pos, glm::vec3 diff,glm::vec3 spe ,glm::vec3 att, glm::vec3 dir, float cutOff, float outterCutOff)
: Light(pos,diff,spe,att), direction(dir)
{
    this->cutOff = glm::cos(glm::radians(cutOff));
    this->outterCutOff = glm::cos(glm::radians(outterCutOff));
}
Reflector::~Reflector()
{
}
bool Reflector::getIsOn()const {return isOn;}
void Reflector::switchLight(){isOn = !isOn;
    std::cout << isOn << std::endl;
}
void Reflector::update(float dt)
{
}
glm::vec3 Reflector::getDirection() const{return this->direction;}
float Reflector::getCutOff() const{return this->cutOff;}
float Reflector::getOutterCutOff() const{return this->outterCutOff;}
void Reflector::FlashMovement(Camera* camera){
    float offsetDistance = 0.5f;
    glm::vec3 lightPos = camera->getPosition() + camera->getTarget() * offsetDistance;
    setPosition(lightPos);
    direction = camera->getTarget();
}