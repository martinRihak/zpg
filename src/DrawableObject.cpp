#include "DrawableObject.hpp"
#include "Transformation/ITransformation.hpp"
#include "IAnimator/IAnimator.hpp"
#include "Lights/PointLight.hpp"
#include <limits> // Pro FLT_MAX

DrawableObject::DrawableObject(Model *model, ShaderProgram *shader)
    : model(model), shader(shader), material(new Material())
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
void DrawableObject::draw(float dt)
{
    shader->use();
    shader->setUniform("lightCount", 0);
    update(dt);
    shader->updateMaterial(this->material);
    this->shader->setModelMatrix(tranformation->getModelMatrix());
    model->draw();
    glUseProgram(0);
}

void DrawableObject::createRandomMovement(float speed, float baseInterval)
{
    this->animator = std::make_unique<RandomMovementAnimator>(speed, baseInterval);
    this->animated = true;
}
void DrawableObject::draw(float dt, const std::vector<Light *> &lights)
{
    if (material->isSkyBoxMaterial())
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
    glBindTexture(GL_TEXTURE_CUBE_MAP, material->getTextureID());
    this->shader->setUniform("cubeTexture", 0);
    this->shader->setUniform("time", material->getTime());
    update(dt);
    model->draw();
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

    shader->updateMaterial(this->material);

    update(dt);
    model->draw();

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
    newObj->setMaterial(this->material);
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
    if (material)
        delete material;
    material = mat;
}
void DrawableObject::createMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s, float shiness)
{
    this->material = new Material(a, d, s, shiness);
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

    // 3. Kontrola, zda se intervaly překrývají a zda je průsečík před kamerou
    if (tmax < 0 || tmin > tmax)
        return false;

    dist = tmin > 0.0f ? tmin : tmax; // Pokud tmin < 0, ale tmax > 0 → jsme uvnitř boxu
    return true;
}
bool DrawableObject::collidesWith(const DrawableObject *other) const
{
    glm::vec3 distVec = getTransformation().getPosition() - other->getTransformation().getPosition();
    float dist = glm::length(distVec);
    float sumRadius = boundingRadius + other->boundingRadius; // boundingRadius přidej dříve
    return dist <= sumRadius;
}