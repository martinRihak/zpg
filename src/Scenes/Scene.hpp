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
protected:
    std::vector<DrawableObject *> objects;
    std::vector<Light *> lights;
    glm::vec3 background = glm::vec3(0.0f, 0.0f, 0.0f);

    int countObjects = 1;
    bool isGame = false;
public:
    Scene();
    ~Scene();
    void addObject(DrawableObject *obj);
    void addLight(Light *light);

    void setBackground(const glm::vec3 &color) { background = color; }
    const glm::vec3 &getBackground() const { return background; }
    const std::vector<Light *> &getLights() const;
    void randomForest(glm::vec3 center, int radius, const std::vector<std::pair<DrawableObject *, int>> &objectsToSpawn);
    virtual void render(float dt);

    bool isGameScene() { return isGame; }

    DrawableObject *pickObject(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, float &dist);
    std::vector<DrawableObject *> &getObjects() { return objects; } // Pro mazání

    DrawableObject *getObjectById(uint8_t id);
};
