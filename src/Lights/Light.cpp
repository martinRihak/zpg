#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s) : diff(d), spec(s),att(glm::vec3(1.0f, 0.0f, 0.0f))
{
    transformation = std::make_unique<Transformation>(); 
    transformation->setPosition(pos);
}
Light::Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s,glm::vec3 a) : diff(d), spec(s), att(a)
{
    transformation = std::make_unique<Transformation>();
    transformation->setPosition(pos);
}

glm::vec3 Light::getPosition() const { return transformation->getPosition(); }

void Light::setPosition(const glm::vec3 &pos)
{
    transformation->setPosition(pos);
}

glm::vec3 Light::getDiff() const { return diff; }

void Light::setDiff(const glm::vec3 &d)
{
    diff = d;
}

glm::vec3 Light::getSpec() const { return spec; }

void Light::setSpec(const glm::vec3 &s)
{
    spec = s;
}
glm::vec3 Light::getAtt() const { return att; }

void Light::attachTransformation(Transformation& trans){
    this->transformation = std::make_unique<Transformation>(trans);
}
Transformation &Light::getTransformation()
{
    return *transformation;
}
const Transformation &Light::getTransformation() const
{
    return *transformation;
}