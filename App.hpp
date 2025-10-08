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
//Model
#include "Models/bushes.h"
// Add explicit includes for each required model header file below:
#include "Models/tree.h"
#include "Models/gift.h"
#include "Models/suzi_flat.h"
#include "Models/plain.h"
#include "Models/sphere.h"

// #include "Models/another_model.hpp" // Add more as needed

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
    
    //void createShaderProgram(const char* vertexShader,const char* fragmentShader);
    //void createShaderProgram(Shader* vertexShader,Shader* fragmentShader);
    void addShaderProgram(ShaderProgram* program);

    int8_t getSceneCount();
    int8_t activeScene();
    void run();
};