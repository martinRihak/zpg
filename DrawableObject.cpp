#include "DrawableObject.hpp"


DrawableObject::DrawableObject(Model* model,ShaderProgram* shader) 
    : model(model),shader(shader){}

DrawableObject::~DrawableObject(){
    delete(this->model);
    delete(this->shader);
}

void DrawableObject::draw(){
    shader->use();
    this->shader->setModelMatrix(tranformation.getModelMatrix());
    model->draw();
}

Transformation& DrawableObject::getTransformation() {
    return this->tranformation;
}