#pragma once

#include <vector>
#include <cstdint>
#include "Scenes/Scene.hpp"
#include "Camera.hpp"
#include "Scenes/SceneBuilder.hpp"

class Scene;
class SceneBuilder;
class Controller
{
public:
    Controller();
    ~Controller();
    virtual void handleMouseClick(GLFWwindow *window, int button, int action, int mods, Camera *camera, Scene *scene, SceneBuilder *builder);
    virtual void processInput(GLFWwindow *window, int8_t sceneCount, Camera *camera, float dt, SceneBuilder *builder);
    int8_t getActiveScene();
    Scene *getCurrentScene() { return currentScene; }
    void setCurrentScene(Scene *scene) { currentScene = scene; }

private:
    bool prevKeyState[9] = {false};
    float lastX, lastY;
    bool isRightMouseButtonPressed = false;
    int8_t active = 0;
    DrawableObject *selectedObject = nullptr;
    bool isDragging = false;
    glm::vec2 lastMousePos;
    Scene *currentScene = nullptr;

    bool bezierMode = false; 
    bool prevBState = false;
};