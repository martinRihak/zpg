#include "Directional.hpp"

Directional::Directional(glm::vec3 pos,glm::vec3 diff,glm::vec3 att,glm::vec3 dir):
Light(pos,diff,att) , direction(dir){}
Directional::~Directional(){}

void Directional::update(float dt) {
 
}

glm::vec3 Directional::getDirection() const {return this->direction;}

void Directional::setDirection(const glm::vec3 &dir) {
    direction = glm::normalize(dir);
    this->getTransformation().setPosition(-direction);  

}