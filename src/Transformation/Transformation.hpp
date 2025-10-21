#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "ITransformation.hpp"

class Move : public ITransformation {
private:
    glm::vec3 position;
public:
    Move() : position(0.0f, 0.0f, 0.0f) {}
    void setPosition(const glm::vec3& pos) { position = pos; }
    glm::vec3 getPosition() const { return position; }
    glm::mat4 getModelMatrix() const override {
        return glm::translate(glm::mat4(1.0f), position);
    }
};

class Rotate : public ITransformation {
private:
    float angle;
    glm::vec3 axis;
public:
    Rotate() : angle(0.0f), axis(0.0f, 1.0f, 0.0f) {}
    void setRotation(float ang, const glm::vec3& ax) { angle = ang; axis = ax; }
    float getAngle() const { return angle; }
    glm::vec3 getAxis() const { return axis; }
    glm::mat4 getModelMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    }
};

class Scale : public ITransformation {
private:
    glm::vec3 scl;
public:
    Scale() : scl(1.0f, 1.0f, 1.0f) {}
    void setScale(const glm::vec3& s) { scl = s; }
    glm::vec3 getScale() const { return scl; }
    glm::mat4 getModelMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scl);
    }
};

class Transformation {
private:
    std::shared_ptr<Move> move;
    std::shared_ptr<Rotate> rotate;
    std::shared_ptr<Scale> scale;
    glm::mat4 modelMatrix;

    void updateModelMatrix();

public:
    Transformation();
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition() const;
    void setRotation(float angle, const glm::vec3& axis);
    float getRotationAngle() const;
    glm::vec3 getRotationAxis() const;
    void setScale(const glm::vec3& scl);
    glm::vec3 getScale() const;
    glm::mat4 getModelMatrix() const;
    void setModelMatrix(const glm::mat4& mat);
};