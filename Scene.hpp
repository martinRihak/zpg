#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "DrawableObject.hpp"

class Scene
{
private:
    std::vector<DrawableObject*> objects;
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void runScene();
};
