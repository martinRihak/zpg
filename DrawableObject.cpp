#include "DrawableObject.hpp"


DrawableObject::DrawableObject(Model* model,ShaderProgram* shader) 
    : model(model),shader(shader){}

DrawableObject::~DrawableObject(){
    delete(this->model);
    delete(this->shader);
}

void DrawableObject::draw(){
    shader->use();
    model->draw();
    
}