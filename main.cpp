// Include GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

// Include the standard C++ headers
#include <stdlib.h>
#include <stdio.h>
#include "App.hpp"
//my classes
bool rotateLeft = false;
float maxSpeed = 50.f;
static void error_callback(int error, const char *description) { fputs(description, stderr); }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == 265 && action == GLFW_PRESS) // L key
        rotateLeft = !rotateLeft;
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

static void button_callback(GLFWwindow *window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
        printf("button_callback [%d,%d,%d]\n", button, action, mode);
}
// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
int main(void)
{
    App* app = new App(800,600);
    app->run();

}