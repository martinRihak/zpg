#include "CallbackHandler.hpp"
#include <cstdio>  // Pro printf

CallbackHandler::CallbackHandler(GLFWwindow* win, App* application) : window(win), app(application) {
    // Registrace callbacků
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, button_callback);
}

CallbackHandler::~CallbackHandler() {
    glfwSetErrorCallback(nullptr);
    glfwSetKeyCallback(window, nullptr);
    glfwSetWindowSizeCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
}

void CallbackHandler::error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

void CallbackHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("key_callback [%d, %d, %d, %d]\n", key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void CallbackHandler::window_size_callback(GLFWwindow* window, int width, int height) {
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
    
    float newAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    // Získat App z user pointer
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if (app) {
        Camera* camera = app->getCamera();  // Předpokládám, že máš getter getCamera()
        if (camera) {
            camera->setAspectRatio(newAspectRatio);  // Nastaví aspect a notifikuje shadery
        }
    }
}

void CallbackHandler::button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        printf("button_callback [%d, %d, %d]\n", button, action, mods);
    }
}