#include "DrawableObject.hpp"
#include "Transformation/ITransformation.hpp"
#include "Lights/PointLight.hpp"
DrawableObject::DrawableObject(Model *model, ShaderProgram *shader)
    : model(model), shader(shader), material(new Material())
{
    tranformation = new Transformation();
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::draw(float dt)
{
    shader->use();
    shader->setUniform("lightCount", 0);
    update(dt);
    shader->updateMaterial(this->material);
    this->shader->setModelMatrix(tranformation->getModelMatrix());
    if(this->material->hasTexture() == true){

    }
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
    shader->use();
    shader->setUniform("lightCount", static_cast<int>(lights.size()));
    for (int i = 0; i < lights.size() && i < 8; i++)
    {
        lights[i]->update(dt);
        shader->updateLight(i, lights[i]);
        shader->updateMaterial(this->material);
    }
    update(dt);
    shader->setModelMatrix(tranformation->getModelMatrix());
    model->draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void DrawableObject::createRotation(float speedDegPerSec, glm::vec3 axis, int dir)
{
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis, dir);
    this->animated = true;
}
void DrawableObject::createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg)
{
    animator = std::make_unique<OrbitAnimator>(center, radius, speedDegPerSec, initialAngleDeg);
    setAnimated(true);
}

Transformation &DrawableObject::getTransformation()
{
    return *tranformation;
}
void DrawableObject::update(float dt)
{
    if (animated && animator)
    {
        animator->update(*tranformation, dt);
    }
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
    this->light = new PointLight(this->getTransformation().getPosition(),
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