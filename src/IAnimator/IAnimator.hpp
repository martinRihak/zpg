#pragma once
#include "../Transformation/Transformation.hpp"
#include "IAnimatable.hpp"

class IAnimator
{
public:
    virtual ~IAnimator() = default;
    virtual void update(IAnimatable &obj, float dt) = 0;
};
class NullAnimator : public IAnimator
{
public:
    void update(IAnimatable &obj, float dt) override;
};

class RotateAnimator : public IAnimator
{
public:
    RotateAnimator(float speedDegPerSec, glm::vec3 axis, int dir);
    void update(IAnimatable &obj, float dt) override;

private:
    float speed;
    int dir;
    glm::vec3 axis;
    float angle;
};
class OrbitAnimator : public IAnimator
{
public:
    OrbitAnimator(const IAnimatable *center, float radius, float speedDegPerSec, float initialAngleDeg = 0.0f);
    void update(IAnimatable &obj, float dt) override;

private:
    const IAnimatable *center;
    float radius;
    float speed;
    float angle;
};

class RandomMovementAnimator : public IAnimator
{
private:
    float speed;
    float timeSinceLastChange;
    float baseChangeInterval;
    glm::vec3 direction;
    glm::vec3 maxBounds;
    glm::vec3 minBounds;
    void randomizeDirection();
    bool isInBounds(const glm::vec3 &position) const;

public:
    RandomMovementAnimator(float speed, float baseChangeInterval,
                           glm::vec3 minBounds = glm::vec3(-5.0f, 0.0f, -5.0f),
                           glm::vec3 maxBounds = glm::vec3(5.0f));
    void update(IAnimatable &obj, float dt) override;
};
class MoveBetweenPointsAnimator : public IAnimator
{
public:
    MoveBetweenPointsAnimator(glm::vec3 pointA, glm::vec3 pointB, float speed);

    void update(IAnimatable &obj, float dt) override;

private:
    glm::vec3 A, B;
    float speed;
    bool goingToB;
};
class Camera; 
class ShootAnimator : public IAnimator
{
private:
    glm::vec3 startPos;
    glm::vec3 direction; // Norm vector
    float speed;
    bool initialized;
    float radius;
    Camera *camera;

public:
    ShootAnimator(glm::vec3 startPos, glm::vec3 direction, float speed , Camera *camera);
    void update(IAnimatable &obj, float dt) override;
};
class ApproachCameraAnimator : public IAnimator
{
private:
    const Camera *camera;
    float speed;
    float radius;

public:
    ApproachCameraAnimator(const Camera *cam, float speed,  float radius);
    void update(IAnimatable &obj, float dt) override;
};

class BasicBezier : public IAnimator
{
private:
    glm::mat4 points;
    glm::mat4x3 B;
    float t;
    float speed = 1.f;
    float delta = 0.5f;

public:
    BasicBezier(glm::mat4 points,glm::mat4x3 B,float t);
    void update(IAnimatable &obj, float dt) override;
};