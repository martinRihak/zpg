#include "Scene.hpp"

Scene::Scene(){
}

Scene::~Scene(){

}
void Scene::addObject(DrawableObject* obj){
    this->objects.push_back(obj);
}

void Scene::render(float dt){
    for(DrawableObject* o : this->objects){
        o->draw(dt); 
    }
}