#pragma once
#include "../Transformation/Transformation.hpp"

// Forward declaration třídy DrawableObject, aby se vyřešila závislost
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
    DrawableObject* center;  // Centrální objekt, kolem kterého obíháme (např. Slunce pro Zemi).
    float radius;            // Poloměr orbity (vzdálenost od centra).
    float speed;             // Úhlová rychlost v stupních za sekundu.
    float angle;             // Aktuální úhel v stupních.
};