#include "Transformation.hpp"

Transformation::Transformation()
    : position(0.0f), scale(1.0f), rotationAxis(0.0f, 1.0f, 0.0f), rotationAngle(0.0f) {
    updateModelMatrix();
}

void Transformation::setPosition(const glm::vec3& pos) {
    position = pos;
    updateModelMatrix();
}

void Transformation::setRotation(float angle, const glm::vec3& axis) {
    rotationAngle = angle;
    rotationAxis = axis;
    updateModelMatrix();
}

void Transformation::setScale(const glm::vec3& scl) {
    scale = scl;
    updateModelMatrix();
}

glm::mat4 Transformation::getModelMatrix() const {
    return modelMatrix;
}

void Transformation::setModelMatrix(const glm::mat4& mat) {
    modelMatrix = mat;
    // optionally decompose mat into position/rotation/scale if needed later
}

void Transformation::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, scale);
}
