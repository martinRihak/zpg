#pragma once

#include <vector>
#include <cstdint>
#include "Scene.hpp"
#include "Camera.hpp"
class Scene;

class Controller {
public:
    Controller();
    ~Controller();

    void processInput(GLFWwindow* window, std::vector<Scene*>& scenes, int8_t &active, int8_t sceneCount,Camera* camera);

private:
    // simple debounce state for keys 1..9
    bool prevKeyState[9];
    float lastX, lastY;
    bool isRightMouseButtonPressed = false;
};
