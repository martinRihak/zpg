#pragma once

#include <vector>
#include <cstdint>
#include "Scenes/Scene.hpp"
#include "Camera.hpp"
#include "Scenes/SceneBuilder.hpp"  // Přidáno pro SceneBuilder

class Scene;

class Controller
{
public:
    Controller();
    ~Controller();
    void handleMouseClick(GLFWwindow *window, int button, int action, int mods, Camera *camera, Scene *scene, SceneBuilder *builder);
    void processInput(GLFWwindow *window, int8_t sceneCount, Camera *camera, float dt, SceneBuilder *builder);  // Upraveno: Sloučeno, přidán builder
    int8_t getActiveScene();

private:
    bool prevKeyState[9] = {false};
    float lastX, lastY;
    bool isRightMouseButtonPressed = false;
    int8_t active = 0;

    DrawableObject *selectedObject = nullptr;
    bool isDragging = false;
    glm::vec2 lastMousePos;
};