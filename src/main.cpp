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

int main(void)
{
    App* app = new App(1280,720);
    app->run();

}