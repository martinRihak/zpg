#include "DrawableObject.hpp"
#include "Transformation/ITransformation.hpp"

DrawableObject::DrawableObject(Model *model, ShaderProgram *shader)
    : model(model), shader(shader) {}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::draw(float dt)
{
    shader->use();
    shader->setUniform("viewPos", shader->getCameraPos());
    shader->setUniform("lightCount", 0);
    update(dt);
    this->shader->setModelMatrix(tranformation.getModelMatrix());
    model->draw();
}

void DrawableObject::draw(float dt, const std::vector<Light *> &lights)
{
    shader->use();
    shader->setUniform("lightCount", static_cast<int>(lights.size()));
    for (int i = 0; i < lights.size() && i < 8; i++)
    {
        shader->updateLight(i, lights[i]);
    }
    shader->setUniform("viewPos", shader->getCameraPos());
    update(dt);
    shader->setModelMatrix(tranformation.getModelMatrix());
    model->draw();
}
void DrawableObject::createRotation(float speedDegPerSec, glm::vec3 axis)
{
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis);
    this->animated = true;
}
void DrawableObject::createOrbit(DrawableObject *center, float radius, float speedDegPerSec, float initialAngleDeg)
{
    animator = std::make_unique<OrbitAnimator>(center, radius, speedDegPerSec, initialAngleDeg);
    setAnimated(true);
}

Transformation &DrawableObject::getTransformation()
{
    return this->tranformation;
}
void DrawableObject::update(float dt)
{
    if (animated && animator)
    {
        animator->update(this->tranformation, dt);
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
    this->tranformation.setModelMatrix(composed);
    queuedTransforms = CompositeTransformation();
}
DrawableObject *DrawableObject::clone() const
{

    DrawableObject *newObj = new DrawableObject(this->model, this->shader);

    newObj->tranformation = this->tranformation;

    return newObj;
}