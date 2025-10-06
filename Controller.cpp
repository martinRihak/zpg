#include "Controller.hpp"

Controller::Controller() {
    for (int i = 0; i < 9; ++i) prevKeyState[i] = false;
}

Controller::~Controller() {}

void Controller::processInput(GLFWwindow* window, std::vector<Scene*>& scenes, int8_t &active, int8_t sceneCount) {
    if (!window) return;
    for (int i = 0; i < 9; ++i) {
        int key = GLFW_KEY_1 + i; // GLFW_KEY_1 .. GLFW_KEY_9
        int state = glfwGetKey(window, key);
        bool pressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        if (pressed && !prevKeyState[i]) {
            // first press -> set active if within range
            if (i < sceneCount) {
                active = static_cast<int8_t>(i);
            }
        }
        prevKeyState[i] = pressed;
    }
}
