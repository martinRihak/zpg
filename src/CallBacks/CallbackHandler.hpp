#pragma once

#include <vector>
#include "../App.hpp" 
#include "../shaderProgram/ShaderProgram.hpp" 
class App;
class CallbackHandler {
private:
    GLFWwindow* window;

public:
    CallbackHandler(GLFWwindow* win);
    ~CallbackHandler();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void button_callback(GLFWwindow* window, int button, int action, int mods);
};