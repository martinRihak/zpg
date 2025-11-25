#pragma once

#include "Controller.hpp"


class GameController : public Controller
{
private:
    
public:
    GameController();
    ~GameController();  
    void handleMouseClick(GLFWwindow *window, int button, int action, int mods, Camera *camera, Scene *scene, SceneBuilder *builder) override;
    void processInput(GLFWwindow *window, int8_t sceneCount, Camera *camera, float dt, SceneBuilder *builder) override;  
};