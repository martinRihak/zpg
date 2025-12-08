#include "Transformation.hpp"

Transformation::Transformation()
    : move(std::make_unique<Move>()),
      rotate(std::make_unique<Rotate>()),
      scale(std::make_unique<Scale>()),
      custom(std::make_unique<CustomTransform>()),
      composite(nullptr),
      useCustom(false),
      dirty(true)
{
}

Transformation::Transformation(const Transformation& other)
    : move(std::make_unique<Move>(*other.move)),
      rotate(std::make_unique<Rotate>(*other.rotate)),
      scale(std::make_unique<Scale>(*other.scale)),
      custom(std::make_unique<CustomTransform>(*other.custom)),
      composite(other.composite),
      useCustom(other.useCustom),
      dirty(true)
{
}

Transformation& Transformation::operator=(const Transformation& other)
{
    if (this != &other)
    {
        move = std::make_unique<Move>(*other.move);
        rotate = std::make_unique<Rotate>(*other.rotate);
        scale = std::make_unique<Scale>(*other.scale);
        custom = std::make_unique<CustomTransform>(*other.custom);
        composite = other.composite;
        useCustom = other.useCustom;
        dirty = true;
    }
    return *this;
}

void Transformation::updateModelMatrix()
{
    modelMatrix = move->getModelMatrix() * rotate->getModelMatrix() * scale->getModelMatrix();
    if (useCustom)
        modelMatrix = modelMatrix * custom->getModelMatrix();
}

void Transformation::setPosition(const glm::vec3& pos)
{
    move->setPosition(pos);
    dirty = true;
}

glm::vec3 Transformation::getPosition() const
{
    return move->getPosition();
}

void Transformation::setRotation(float angle, const glm::vec3& axis)
{
    rotate->setRotation(angle, axis);
    dirty = true;
}

void Transformation::setRotation(const glm::mat4 &mat)
{
    rotate->setRotation(mat);
    dirty = true;
}

float Transformation::getRotationAngle() const
{
    return rotate->getAngle();
}

glm::vec3 Transformation::getRotationAxis() const
{
    return rotate->getAxis();
}

void Transformation::setScale(const glm::vec3& scl)
{
    scale->setScale(scl);
    dirty = true;
}

glm::vec3 Transformation::getScale() const
{
    return scale->getScale();
}

glm::mat4 Transformation::getModelMatrix()
{
    if (dirty)
    {
        updateModelMatrix();
        dirty = false;
    }
    return modelMatrix;
}

void Transformation::setModelMatrix(const glm::mat4& mat)
{
    modelMatrix = mat;
    dirty = false;
}