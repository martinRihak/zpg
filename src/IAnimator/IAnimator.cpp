#include "IAnimator.hpp"
#include "IAnimatable.hpp"
#include "../Camera.hpp"
#include <glm/gtx/transform.hpp>
#include <random> // Pro náhodnost
#include <ctime>  // Pro seedování randomu
#include <glm/gtx/string_cast.hpp>
#include <glm/exponential.hpp>
void NullAnimator::update(IAnimatable &obj, float dt)
{
}

RotateAnimator::RotateAnimator(float speedDegPerSec, glm::vec3 axis, int dir)
    : speed(speedDegPerSec), axis(axis), angle(0.f), dir(dir) {}

void RotateAnimator::update(IAnimatable &obj, float dt)
{
    angle += (speed * dt) * dir;
    obj.getTransformation().setRotation(angle, axis);
}

OrbitAnimator::OrbitAnimator(const IAnimatable *center, float radius, float speedDegPerSec, float initialAngleDeg)
    : center(center), radius(radius), speed(speedDegPerSec), angle(initialAngleDeg)
{
}

void OrbitAnimator::update(IAnimatable &obj, float dt)
{
    angle += speed * dt;

    glm::vec3 centerPos = center->getTransformation().getPosition();

    float rad = glm::radians(angle);
    glm::vec3 offset(radius * cos(rad), 0.0f, radius * sin(rad));
    glm::vec3 newPos = centerPos + offset;

    obj.getTransformation().setPosition(newPos);
}

RandomMovementAnimator::RandomMovementAnimator(float speed, float baseChangeInterval,
                                               glm::vec3 minBounds, glm::vec3 maxBounds)
    : speed(speed), timeSinceLastChange(0.0f), baseChangeInterval(baseChangeInterval),
      minBounds(minBounds), maxBounds(maxBounds)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    randomizeDirection();
}

bool RandomMovementAnimator::isInBounds(const glm::vec3 &position) const
{
    return position.x >= minBounds.x && position.x <= maxBounds.x &&
           position.y >= minBounds.y && position.y <= maxBounds.y &&
           position.z >= minBounds.z && position.z <= maxBounds.z;
}

void RandomMovementAnimator::randomizeDirection()
{
    // Pro robustnější řešení zvažte použití <random> knihovny (např. std::mt19937)
    direction = glm::normalize(glm::vec3(
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f,
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f,
        static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f));
}

void RandomMovementAnimator::update(IAnimatable &obj, float dt)
{
    Transformation &t = obj.getTransformation();
    timeSinceLastChange += dt;

    float effectiveInterval = baseChangeInterval + (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * 0.5f;
    if (timeSinceLastChange > effectiveInterval)
    {
        randomizeDirection();
        timeSinceLastChange = 0.0f;
    }
    glm::vec3 delta = direction * speed * dt;
    glm::vec3 newPosition = t.getPosition() + delta;

    if (!isInBounds(newPosition))
    {
        randomizeDirection();
        if (newPosition.x < minBounds.x || newPosition.x > maxBounds.x)
            direction.x *= -1;
        if (newPosition.y < minBounds.y || newPosition.y > maxBounds.y)
            direction.y *= -1;
        if (newPosition.z < minBounds.z || newPosition.z > maxBounds.z)
            direction.z *= -1;
        newPosition = glm::clamp(newPosition, minBounds, maxBounds);
    }

    t.setPosition(newPosition);
}
MoveBetweenPointsAnimator::MoveBetweenPointsAnimator(glm::vec3 pointA, glm::vec3 pointB, float speed)
    : A(pointA), B(pointB), speed(speed), goingToB(true) {}

void MoveBetweenPointsAnimator::update(IAnimatable &obj, float dt)
{
    Transformation &t = obj.getTransformation();
    glm::vec3 currentPos = t.getPosition();
    glm::vec3 target = (goingToB ? B : A);

    glm::vec3 direction = glm::normalize(target - currentPos);
    glm::vec3 nextPos = currentPos + direction * speed * dt;

    if (glm::dot(target - nextPos, direction) < 0.0f)
    {
        nextPos = target;
        goingToB = !goingToB;
    }

    t.setPosition(nextPos);
}
ShootAnimator::ShootAnimator(glm::vec3 startPos, glm::vec3 direction, float speed, Camera *camera)
    : startPos(startPos), direction(direction), speed(speed), initialized(false), camera(camera), radius(20.0f) {}

void ShootAnimator::update(IAnimatable &obj, float dt)
{
    Transformation &t = obj.getTransformation();

    if (!initialized)
    {
        t.setPosition(startPos);
        initialized = true;
    }
    glm::vec3 currentPos = t.getPosition();
    glm::vec3 cameraPos = camera->getPosition();

    float distanceToCamera = glm::distance(currentPos, cameraPos);

    if (distanceToCamera >= radius)
    {
        obj.destroy();
        return;
    }
    glm::vec3 newPos = currentPos + direction * speed * dt;
    t.setPosition(newPos);
}
ApproachCameraAnimator::ApproachCameraAnimator(const Camera *cam, float speed, float radius)
    : camera(cam), speed(speed), radius(radius) {} // Opravená inicializace: přidán radius
void ApproachCameraAnimator::update(IAnimatable &obj, float dt)
{
    Transformation &t = obj.getTransformation();
    if (!camera)
        return;

    glm::vec3 currentPos = t.getPosition();
    glm::vec3 cameraPos = camera->getPosition();

    float distanceToCamera = glm::distance(currentPos, cameraPos);

    if (distanceToCamera <= radius)
    {
        obj.destroy();
        return;
    }

    glm::vec3 direction = glm::normalize(cameraPos - currentPos);
    glm::vec3 newPos = currentPos + direction * speed * dt;
    t.setPosition(newPos);
}
BasicBezier::BasicBezier(glm::mat4 points, glm::mat4x3 B, float t) : points(points), B(B), t(t) {}

void BasicBezier::update(IAnimatable &obj, float dt)
{
    Transformation &tr = obj.getTransformation();

    glm::vec4 parameters = glm::vec4(t * t * t, t * t, t, 1.0f);
    glm::vec3 p = parameters * points * glm::transpose(B);
    tr.setPosition(glm::vec3(p));

    if (t >= 1.0f || t <= 0.0)
        delta *= -1;
    t += delta * dt;
}
BezierAnimator::BezierAnimator(std::vector<glm::vec3> points, float speed) : points(points), speed(speed)
{
    if ((points.size() - 1) % 3 != 0)
    {
        throw std::invalid_argument("Neplatný počet bodů pro kubickou spline");
    }
}
BezierAnimator::BezierAnimator(std::vector<glm::vec3> points, float speed, bool rotation) : points(points), speed(speed), rotation(rotation)
{
    if ((points.size() - 1) % 3 != 0)
    {
        throw std::invalid_argument("Neplatný počet bodů pro kubickou spline");
    }
}
glm::vec3 BezierAnimator::compute(const std::vector<glm::vec3> &points, float localT) const
{
    float oneMinusT = 1.0f - localT;

    float oneMinusTCu = glm::pow(oneMinusT, 3.0f);
    float oneMinusTSqLocalT = 3.0f * glm::pow(oneMinusT, 2.0f) * localT;
    float oneMinusTTsq = 3.0f * oneMinusT * glm::pow(localT, 2.0f);
    float tCu = glm::pow(localT, 3.0f);

    return (oneMinusTCu * points[0]) +
           (oneMinusTSqLocalT * points[1]) +
           (oneMinusTTsq * points[2]) +
           (tCu * points[3]);
}

glm::vec3 BezierAnimator::computeTangent(const std::vector<glm::vec3> &points, float localT) const
{
    float oneMinusT = 1.0f - localT;
    return 3.0f * oneMinusT * oneMinusT * (points[1] - points[0]) +
           6.0f * oneMinusT * localT * (points[2] - points[1]) +
           3.0f * localT * localT * (points[3] - points[2]);
}
void BezierAnimator::addPoint(glm::mat3 point)
{
    points.push_back(point[0]);
    points.push_back(point[1]);
    points.push_back(point[2]);
}
void BezierAnimator::update(IAnimatable &obj, float dt)
{
    t += speed * dt;
    int numSegments = (points.size() - 1) / 3;
    if (t > numSegments)
        t = 0.0f;
    if (t < 0.0f)
        t = 0.0f; // Ochrana pro negativní speed, pokud bys ji přidal

    int segment = static_cast<int>(t);
    float localT = t - segment;
    if (segment >= numSegments || (segment * 3 + 3) >= points.size())
    {
        return; // Bezpečnostní návrat
    }

    std::vector<glm::vec3> segPoints(4);
    for (int i = 0; i < 4; ++i)
    {
        segPoints[i] = points[segment * 3 + i];
    }

    glm::vec3 newPos = compute(segPoints, localT);

   // std::cout << "t: " << t << ", Pos: " << newPos.x << "," << newPos.y << "," << newPos.z << std::endl;
    //std::cout << "------------------------------------------------" << std::endl;
    if (rotation)
    {
        glm::vec3 tangent = computeTangent(segPoints, localT);
        if (glm::length(tangent) > 0.001f)
        { 
            tangent = glm::normalize(tangent);
            glm::mat4 rotMat = glm::lookAt(glm::vec3(0.0f), tangent, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 newMat = glm::translate(glm::mat4(1.0f), newPos) * rotMat * glm::scale(glm::mat4(1.0f), obj.getTransformation().getScale());
            obj.getTransformation().setModelMatrix(newMat);
            //std::cout << "t: " << t << ", Pos: " << newPos.x << "," << newPos.y << "," << newPos.z << std::endl;
        }
        else
        {
            // Fallback: Žádná rotace, pokud tangent nulový (vzácné)
        }
    }
    else
    {
        obj.getTransformation().setPosition(newPos);
    }
}