#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transformation/ITransformation.hpp"
class Transformation : public ITransformation {
public:
    Transformation();

    void setPosition(const glm::vec3& position);
    void setRotation(float angle, const glm::vec3& axis);
    void setScale(const glm::vec3& scale);

    glm::mat4 getModelMatrix() const;
    void setModelMatrix(const glm::mat4& mat);

private:
    glm::mat4 modelMatrix;  
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationAngle;

    void updateModelMatrix();
};
