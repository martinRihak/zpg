#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include "DrawableObject.hpp"
#include "Light.hpp"
class Scene
{
private:
    std::vector<DrawableObject*> objects;
    std::vector<Light*> lights;
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void addLight(Light* light);

    const std::vector<Light*>& getLights() const;
    void randomForest(glm::vec3 center, int radius, const std::vector<std::pair<DrawableObject*, int>>& objectsToSpawn);
    void render(float dt);
};
