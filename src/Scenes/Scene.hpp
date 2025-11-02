#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include "DrawableObject.hpp"
#include "../Lights/Light.hpp"
class Scene
{
private:
    std::vector<DrawableObject*> objects;
    std::vector<Light*> lights;
    glm::vec3 background = glm::vec3(0.0f, 0.0f, 0.0f) ;
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void addLight(Light* light);


    void setBackground(const glm::vec3& color){background = color;}
    const glm::vec3& getBackground() const {return background;}
    const std::vector<Light*>& getLights() const;
    void randomForest(glm::vec3 center, int radius, const std::vector<std::pair<DrawableObject*, int>>& objectsToSpawn);
    void render(float dt);
};
