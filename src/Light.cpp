#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s) : diff(d), spec(s) {
    transformation.setPosition(pos);
}

glm::vec3 Light::getPosition() const { return transformation.getPosition(); }

void Light::setPosition(const glm::vec3& pos) {
    transformation.setPosition(pos);
    notifyAll();
}

glm::vec3 Light::getDiff() const { return diff; }

void Light::setDiff(const glm::vec3& d) {
    diff = d;
    notifyAll();
}

glm::vec3 Light::getSpec() const { return spec; }

void Light::setSpec(const glm::vec3& s) {
    spec = s;
    notifyAll();
}

Transformation& Light::getTransformation() {
    return transformation;
}

Light::~Light() {}