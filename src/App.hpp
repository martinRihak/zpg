#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shaderProgram/Shader.hpp"
#include "CallBacks/CallbackHandler.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Model.hpp"
#include "DrawableObject.hpp"
#include "App.hpp"
#include "Scenes/SceneBuilder.hpp"
#include "Camera.hpp"
#include "Lights/Light.hpp"
// Model
#include "../Models/AllModels.hpp"
class CallbackHandler;
class App
{
private:
    GLFWwindow *window;
    SceneBuilder* builder;
    int width, height;
    class Controller *controller = nullptr;
    class Camera *camera = nullptr;
    CallbackHandler *callbackHandler;
    void createScenes();
public:
    App(int width, int height);
    ~App();
    Camera *getCamera() { return this->camera; }
    void run();
};