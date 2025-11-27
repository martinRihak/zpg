#include "Transformation.hpp"

Transformation::Transformation()
    : move(std::make_shared<Move>()), rotate(std::make_shared<Rotate>()), scale(std::make_shared<Scale>()),custom(std::make_shared<CustomTransform>()), useCustom(false){
    updateModelMatrix();
}

void Transformation::updateModelMatrix() {

    modelMatrix = move->getModelMatrix() * rotate->getModelMatrix() * scale->getModelMatrix();
    if(useCustom) modelMatrix = modelMatrix * custom->getModelMatrix();
}

void Transformation::setPosition(const glm::vec3& pos) {
    move->setPosition(pos);
    updateModelMatrix();
}

glm::vec3 Transformation::getPosition() const {
    return move->getPosition();
}

void Transformation::setRotation(float angle, const glm::vec3& axis) {
    rotate->setRotation(angle, axis);
    updateModelMatrix();
}
void Transformation::setRotation(const glm::mat4 &mat)
{
    rotate->setRotation(mat);
    updateModelMatrix();
}
float Transformation::getRotationAngle() const {
    return rotate->getAngle();
}

glm::vec3 Transformation::getRotationAxis() const {
    return rotate->getAxis();
}

void Transformation::setScale(const glm::vec3& scl) {
    scale->setScale(scl);
    updateModelMatrix();
}

glm::vec3 Transformation::getScale() const {
    return scale->getScale();
}

glm::mat4 Transformation::getModelMatrix() const {
    return modelMatrix;
}

void Transformation::setModelMatrix(const glm::mat4& mat) {
    modelMatrix = mat;
}