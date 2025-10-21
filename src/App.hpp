#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<vector>

#include "shaderProgram/Shader.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Model.hpp"
#include "DrawableObject.hpp"
#include "App.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Light.hpp"
//Model
#include "../Models/AllModels.hpp"

class App
{
private:
    std::vector<Scene*> scenes;
    GLFWwindow* window;
    int width,height;
    int8_t active = 0;
    int8_t sceneCount = 0;
    class Controller* controller = nullptr;
    class Camera* camera = nullptr;   
public:
    App(int width, int height);
    ~App();
    void createScene();
    void addObjectToScene(DrawableObject* obj, int8_t id);
    void addShaderProgram(ShaderProgram* program);
    Scene* getScene(int i);
    int8_t getSceneCount();
    int8_t activeScene();
    void run();
};