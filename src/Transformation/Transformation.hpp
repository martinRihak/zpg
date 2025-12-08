#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "ITransformation.hpp"

class Move : public ITransformation
{
private:
    glm::vec3 position;

public:
    Move() : position(0.0f, 0.0f, 0.0f) {}
    void setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 getPosition() const { return position; }
    glm::mat4 getModelMatrix() const override
    {
        return glm::translate(glm::mat4(1.0f), position);
    }
};

class Rotate : public ITransformation
{
private:
    float angle;
    glm::vec3 axis;
    glm::mat4 rotationMatrix;
    bool mat4Rotation = false;

public:
    Rotate() : angle(0.0f), axis(0.0f, 1.0f, 0.0f) {}
    void setRotation(float ang, const glm::vec3 &ax)
    {
        angle = ang;
        axis = ax;
    }
    void setRotation(const glm::mat4 &mat)
    {
        rotationMatrix = mat;
        mat4Rotation = true;
    }
    float getAngle() const { return angle; }
    glm::vec3 getAxis() const { return axis; }
    glm::mat4 getModelMatrix() const override
    {
        if (mat4Rotation)
        {
            return rotationMatrix;
        }
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    }
};

class Scale : public ITransformation
{
private:
    glm::vec3 scl;

public:
    Scale() : scl(1.0f, 1.0f, 1.0f) {}
    void setScale(const glm::vec3 &s) { scl = s; }
    glm::vec3 getScale() const { return scl; }
    glm::mat4 getModelMatrix() const override
    {
        return glm::scale(glm::mat4(1.0f), scl);
    }
};

class Transformation
{
private:
    std::unique_ptr<Move> move;
    std::unique_ptr<Rotate> rotate;
    std::unique_ptr<Scale> scale;
    std::unique_ptr<CustomTransform> custom;
    std::shared_ptr<CompositeTransformation> composite;
    bool useCustom;
    mutable bool dirty;
    glm::mat4 modelMatrix;

    void updateModelMatrix();

public:
    Transformation();
    Transformation(const Transformation& other);
    Transformation& operator=(const Transformation& other);
    Transformation(Transformation&&) noexcept = default;
    Transformation& operator=(Transformation&&) noexcept = default;

    void setPosition(const glm::vec3 &pos);
    glm::vec3 getPosition() const;
    void setRotation(float angle, const glm::vec3 &axis);
    void setRotation(const glm::mat4 &mat);
    float getRotationAngle() const;
    glm::vec3 getRotationAxis() const;
    glm::mat4 getRotationMatrix() const { return rotate->getModelMatrix(); }
    void setScale(const glm::vec3 &scl);
    glm::vec3 getScale() const;
    glm::mat4 getModelMatrix();
    void setModelMatrix(const glm::mat4 &mat);

    void setUseCustom(bool enable) { useCustom = enable; dirty = true; }
    bool getUseCustom() const { return useCustom; }

    CompositeTransformation* getComposite() const { return composite.get(); }
    void createComposite() { composite = std::make_shared<CompositeTransformation>(); dirty = true; }
};