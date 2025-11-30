#include "DrawableObject.hpp"
#include "Transformation/ITransformation.hpp"
#include "IAnimator/IAnimator.hpp"
#include "Lights/PointLight.hpp"
#include <limits> // Pro FLT_MAX

DrawableObject::DrawableObject(Model *model, ShaderProgram *shader)
    : model(model), shader(shader)
{
    tranformation = std::make_shared<Transformation>();
    // Inicializace bounding box
    minBounds = model->getMinBounds();
    maxBounds = model->getMaxBounds();
}

DrawableObject::~DrawableObject()
{
}
void DrawableObject::destroy()
{
    destroyVal = true;
}

void DrawableObject::createRandomMovement(float speed, float baseInterval)
{
    this->animator = std::make_unique<RandomMovementAnimator>(speed, baseInterval);
    this->animated = true;
}

void DrawableObject::createBezier(std::vector<glm::vec3> controlPoints, float speed, bool rotation){
    this->animator = std::make_unique<BezierAnimator>(controlPoints, speed, rotation);
    this->animated = true;
    this->Bezier = true;
}
void DrawableObject::createRandomMovement(float speed, float baseInterval, glm::vec3 minBounds, glm::vec3 maxBounds)
{
    this->animator = std::make_unique<RandomMovementAnimator>(speed, baseInterval, minBounds, maxBounds);
    this->animated = true;
}
void DrawableObject::draw(float dt, const std::vector<Light *> &lights)
{
    if (getMaterial()->isSkyBoxMaterial())
    {
        drawSkybox(dt);
    }
    else
    {
        drawRegular(dt, lights);
    }
}
void DrawableObject::drawSkybox(float dt)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    shader->use();

    shader->setModelMatrix(tranformation->getModelMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, getMaterial()->getTextureID());
    this->shader->setUniform("cubeTexture", 0);
    this->shader->setUniform("time", getMaterial()->getTime());
    update(dt);
    model->drawMesh(0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void DrawableObject::drawRegular(float dt, const std::vector<Light *> &lights)
{

    shader->use();

    shader->setUniform("lightCount", static_cast<int>(lights.size()));
    shader->setModelMatrix(tranformation->getModelMatrix());

    for (int i = 0; i < lights.size() && i < 8; i++)
    {
        lights[i]->update(dt);
        shader->updateLight(i, lights[i]);
    }
    update(dt);
    for (size_t i = 0; i < model->getMeshCount(); i++)
    {
        shader->updateMaterial(model->getMaterial(i));
        model->drawMesh(i);
    }

    glUseProgram(0);
}

void DrawableObject::createRotation(float speedDegPerSec, glm::vec3 axis, int dir)
{
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis, dir);
    this->animated = true;
}
void DrawableObject::createOrbit(const IAnimatable *center, float radius, float speedDegPerSec, float initialAngleDeg)
{
    animator = std::make_unique<OrbitAnimator>(center, radius, speedDegPerSec, initialAngleDeg);
    setAnimated(true);
}
void DrawableObject::createBetweenPoints(glm::vec3 p1, glm::vec3 p2, float speed)
{
    animator = std::make_unique<MoveBetweenPointsAnimator>(p1, p2, speed);
    setAnimated(true);
}

void DrawableObject::update(float dt)
{
    if (animated && animator)
    {
        animator->update(*this, dt);
    }
}

Transformation &DrawableObject::getTransformation()
{
    return *tranformation;
}

const Transformation &DrawableObject::getTransformation() const
{
    return *tranformation;
}

IAnimator *DrawableObject::getAnimator() const
{
    return this->animator.get();
}

void DrawableObject::setAnimated(bool enabled)
{
    this->animated = enabled;
}

bool DrawableObject::isAnimated() const
{
    return this->animated;
}

void DrawableObject::queueTransform(std::shared_ptr<ITransformation> t)
{
    if (!t)
        return;
    queuedTransforms.add(t);
}
ShaderProgram *DrawableObject::getShaderProgram() const { return this->shader; }
void DrawableObject::applyQueuedTransforms()
{
    glm::mat4 composed = queuedTransforms.getModelMatrix();
    this->tranformation->setModelMatrix(composed);
    queuedTransforms = CompositeTransformation();
}
DrawableObject *DrawableObject::clone() const
{

    DrawableObject *newObj = new DrawableObject(this->model, this->shader);
    return newObj;
}

bool DrawableObject::getHasLight() const { return this->hasLight; }
PointLight *DrawableObject::getLight() const { return this->light; }
void DrawableObject::createLight(glm::vec3 diff, glm::vec3 spec, glm::vec3 att)
{
    this->light = new PointLight(tranformation->getPosition(),
                                 diff, spec, att);
    light->attachTransformation(*tranformation);
    hasLight = true;
}
void DrawableObject::setMaterial(Material *mat)
{

    this->model->setMaterial(mat);
}
void DrawableObject::createMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness)
{
    this->model->setMaterial(new Material(a, d, s, shiness));
}

bool DrawableObject::intersectsRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, float &dist) const
{
    glm::vec3 scale = tranformation->getScale();
    glm::vec3 pos = tranformation->getPosition();

    glm::vec3 boundsMin = pos + minBounds * scale;
    glm::vec3 boundsMax = pos + maxBounds * scale;
    const float EPS = 1e-6f;
    glm::vec3 invDir = glm::vec3(
        rayDir.x == 0.0f ? FLT_MAX : 1.0f / rayDir.x,
        rayDir.y == 0.0f ? FLT_MAX : 1.0f / rayDir.y,
        rayDir.z == 0.0f ? FLT_MAX : 1.0f / rayDir.z);

    float t1 = (boundsMin.x - rayOrigin.x) * invDir.x;
    float t2 = (boundsMax.x - rayOrigin.x) * invDir.x;
    float tmin = glm::min(t1, t2);
    float tmax = glm::max(t1, t2);

    t1 = (boundsMin.y - rayOrigin.y) * invDir.y;
    t2 = (boundsMax.y - rayOrigin.y) * invDir.y;
    tmin = glm::max(tmin, glm::min(t1, t2));
    tmax = glm::min(tmax, glm::max(t1, t2));

    t1 = (boundsMin.z - rayOrigin.z) * invDir.z;
    t2 = (boundsMax.z - rayOrigin.z) * invDir.z;
    tmin = glm::max(tmin, glm::min(t1, t2));
    tmax = glm::min(tmax, glm::max(t1, t2));

    if (tmax < 0 || tmin > tmax)
        return false;

    dist = tmin > 0.0f ? tmin : tmax;
    return true;
}
bool DrawableObject::collidesWith(const DrawableObject *other) const
{
    glm::vec3 distVec = getTransformation().getPosition() - other->getTransformation().getPosition();
    float dist = glm::length(distVec);
    float sumRadius = boundingRadius + other->boundingRadius;
    return dist <= sumRadius;
}
void DrawableObject::addControlPoint(glm::vec3 point)
{
    BezierAnimator *bezierAnim = dynamic_cast<BezierAnimator *>(animator.get());
    if (bezierAnim)
    {
        bezierAnim->addPoint(point);
        printf("Bod přidán do Bézier animátoru!\n");
    }
    else
    {
        printf("Objekt nemá Bézier animátor!\n");
    }
}