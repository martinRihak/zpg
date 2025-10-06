#pragma once

#include <vector>
#include <cstdint>
#include "Scene.hpp"
class Scene;

class Controller {
public:
    Controller();
    ~Controller();

    void processInput(GLFWwindow* window, std::vector<Scene*>& scenes, int8_t &active, int8_t sceneCount);

private:
    // simple debounce state for keys 1..9
    bool prevKeyState[9];
};
