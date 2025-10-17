#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 diff, glm::vec3 spec) : position(pos),diff(diff),spec(spec){}

glm::vec3 Light::getPosition() const {return position;}
glm::vec3 Light::getDiff() const {return diff;}
glm::vec3 Light::getSpec() const { return spec;}


