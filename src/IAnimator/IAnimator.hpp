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
    RotateAnimator(float speedDegPerSec, glm::vec3 axis,int dir);
    void update(Transformation &t, float dt) override;
private:
    float speed;
    int dir;
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
private:
    float speed;
    float timeSinceLastChange;
    float baseChangeInterval;
    glm::vec3 direction;
    glm::vec3 maxBounds;  
    glm::vec3 minBounds;  
    void randomizeDirection();
    bool isInBounds(const glm::vec3& position) const;
public:
    RandomMovementAnimator(float speed, float baseChangeInterval, 
                          glm::vec3 minBounds = glm::vec3(-5.0f,0.0f,-5.0f), 
                          glm::vec3 maxBounds = glm::vec3(5.0f));
    void update(Transformation& t, float dt) override;

};
class MoveBetweenPointsAnimator : public IAnimator {
public:
    MoveBetweenPointsAnimator(glm::vec3 pointA, glm::vec3 pointB, float speed);

    void update(Transformation &t, float dt) override;

private:
    glm::vec3 A, B;
    float speed;
    bool goingToB;
};
