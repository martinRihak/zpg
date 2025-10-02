#pragma once
#include "../Transformation.hpp"


class IAnimator {
public:
    virtual ~IAnimator() = default;
    virtual void update(Transformation &t, float dt) = 0;
};

class NullAnimator : public IAnimator {
public:
    void update(Transformation &t, float dt) override {}
};

class RotateAnimator : public IAnimator {
public:
    RotateAnimator(float speedDegPerSec, glm::vec3 axis)
      : speed(speedDegPerSec), axis(axis), angle(0.f) {}
    void update(Transformation &t, float dt) override {
       angle += speed * dt;
       t.setRotation(angle, axis);
    }
private:
    float speed;
    glm::vec3 axis;
    float angle;
};

