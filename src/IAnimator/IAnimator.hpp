#pragma once
#include "../Transformation/Transformation.hpp"

class DrawableObject;

class IAnimator {
public:
    virtual ~IAnimator() = default;
    virtual void update(Transformation &t, float dt) = 0;
};

class NullAnimator : public IAnimator {
public:
    void update(Transformation &t, float dt) override;
};

class RotateAnimator : public IAnimator {
public:
    RotateAnimator(float speedDegPerSec, glm::vec3 axis);
    void update(Transformation &t, float dt) override;
private:
    float speed;
    glm::vec3 axis;
    float angle;
};

class OrbitAnimator : public IAnimator {
public:
    OrbitAnimator(DrawableObject* center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f);
    void update(Transformation &t, float dt) override;
private:
    DrawableObject* center;  
    float radius;            
    float speed;             
    float angle;             
};


class RandomMovementAnimator : public IAnimator {
public:
    RandomMovementAnimator(float speed, float baseChangeInterval);
    void update(Transformation &t, float dt) override;
private:
    float speed;                  
    glm::vec3 direction;          
    float timeSinceLastChange;    
    float baseChangeInterval;     
    void randomizeDirection();    
};