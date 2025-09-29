#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<vector>
#include "Scene.hpp"

class App
{
private:
    std::vector<Scene*> scenes;
    GLFWwindow* window;
    int width,height;
    int8_t active = 0;
    int8_t sceneCount;
public:
    App(int width, int height);
    ~App();
    void addScene(Scene* scene);
    void run();
};