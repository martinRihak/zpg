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

//my classes
#include "shaderProgram/Shader.hpp"
#include "shaderProgram/ShaderProgram.hpp"
#include "Model.hpp"
#include "DrawableObject.hpp"
#include "App.hpp"
#include "Scene.hpp"
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
float points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

float square[] = {
    -1.f, -0.3f, 0.f,  // Bottom left
    -1.f,  0.3f, 0.f,  // Top left
     1.f,  0.3f, 0.f,  // Top right
    // Second triangle
     1.f,  0.3f, 0.f,  // Top right
     1.f, -0.3f, 0.f,  // Bottom right
    -1.f, -0.3f, 0.f   // Bottom left
};
const char *vertex_shader =
    "#version 330\n"
    "layout(location=0) in vec3 vp;"
    "out vec3 pos;"       
    "void main () {"
    "     pos = vec3(vp.x,vp.y,0.5);"
    "     gl_Position = vec4(vp, 1.0);"
    "}";
const char *vertex_shader_02=
    "#version 330\n"
    "layout(location=0) in vec3 vp;"
    "out vec3 pos;"       
    "void main () {"
    "     pos = vec3(vp.x,vp.y,1);"
    "     gl_Position = vec4(vp, 1.0);"
    "}";
const char *fragment_shader =
    "#version 330\n"
    "in vec3 pos;"        
    "out vec4 frag_colour;"
    "void main () {"
    "     frag_colour = vec4(pos, 1.0);}";
// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

// Camera matrix
glm::mat4 View = glm::lookAt(
    glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
    glm::vec3(0, 0, 0),    // and looks at the origin
    glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
);

int main(void)
{
    App* app = new App(800,600);

    Scene* scene1 = new Scene();
    app->addScene(scene1);

    Model* test = new Model(points,sizeof(points),3);
    Model* squareModel = new Model(square,sizeof(square),6);
    Shader* vertexShader = new Shader(vertex_shader, GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(fragment_shader,GL_FRAGMENT_SHADER);
    Shader* vertex02 = new Shader(vertex_shader_02,GL_VERTEX_SHADER);

    ShaderProgram* shaderProgram = new ShaderProgram(*vertexShader,*fragmentShader);
    ShaderProgram* shaderProgram02 = new ShaderProgram(*vertex02,*fragmentShader);
    DrawableObject* triangle = new DrawableObject(test,shaderProgram);
    DrawableObject* squareObject = new DrawableObject(squareModel,shaderProgram02);    
    scene1->addObject(squareObject);
    scene1->addObject(triangle);

    app->run();

}