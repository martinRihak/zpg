#include "Directional.hpp"

Directional::Directional(glm::vec3 pos,glm::vec3 diff,glm::vec3 att,glm::vec3 dir):
Light(pos,diff,att) , direction(dir){}
Directional::~Directional(){}

void Directional::update(float dt) {
    notifyAll();
}

glm::vec3 Directional::getDirection() const {return this->direction;}

void Directional::setDirection(const glm::vec3 &dir) {
    direction = glm::normalize(dir);
    // Mapujeme na position v transformation (pro kompatibilitu se shaderem, často se používá -direction)
    this->getTransformation().setPosition(-direction);  // Negativní pro standardní konvenci světla (směr od zdroje)
    notifyAll();  // Notifikace observerů
}