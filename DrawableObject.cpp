#include "DrawableObject.hpp"

DrawableObject::DrawableObject(Model *model, ShaderProgram *shader)
    : model(model), shader(shader) {}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::draw(float dt)
{
    shader->use();
    update(dt);
    this->shader->setModelMatrix(tranformation.getModelMatrix());
    model->draw();
}
void DrawableObject::createRotarion(float speedDegPerSec, glm::vec3 axis){
    this->animator = std::make_unique<RotateAnimator>(speedDegPerSec, axis);
    this->animated = true;
}
Transformation &DrawableObject::getTransformation()
{
    return this->tranformation;
}
void DrawableObject::update(float dt){
    if(animated && animator){
        animator->update(this->tranformation, dt);
    }
}
IAnimator* DrawableObject::getAnimator() const{
    return this->animator.get();
}

void DrawableObject::setAnimated(bool enabled){
    this->animated = enabled;
}

bool DrawableObject::isAnimated() const{
    return this->animated;
}
