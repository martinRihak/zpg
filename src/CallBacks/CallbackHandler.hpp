#pragma once

#include <vector>
#include "../App.hpp"  // Pro přístup k App (window, camera, shaderPrograms)
#include "../shaderProgram/ShaderProgram.hpp"  // Pro ShaderProgram
class App;
class CallbackHandler {
private:
    GLFWwindow* window;
    App* app;  

public:
    CallbackHandler(GLFWwindow* win, App* application);
    ~CallbackHandler();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void button_callback(GLFWwindow* window, int button, int action, int mods);
};