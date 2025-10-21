#pragma once

#include <vector>
#include <cstdint>
#include "Scenes/Scene.hpp"
#include "Camera.hpp"
class Scene;

class Controller {
public:
    Controller();
    ~Controller();

    void processInput(GLFWwindow* window, int8_t sceneCount,Camera* camera,float dt);
    int8_t getActiveScene();
private:
    // simple debounce state for keys 1..9
    bool prevKeyState[9];
    float lastX, lastY;
    bool isRightMouseButtonPressed = false;
    int8_t active = 0;
};
