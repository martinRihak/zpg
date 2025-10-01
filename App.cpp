#include "App.hpp"

App::App(int width, int height) : width(width), height(height)
{

    if (!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW3\n";
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(width, height, "ZPG", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
}
App::~App()
{
    for (auto *a : this->scenes)
    {
        delete a;
    }
    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void App::addScene(Scene *scene)
{
    this->scenes.push_back(scene);
}

void App::run()
{
    float points[] = {
        0.0f,
        0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,
    };

    float square[] = {
        -1.f, -0.3f, 0.f, // Bottom left
        -1.f, 0.3f, 0.f,  // Top left
        1.f, 0.3f, 0.f,   // Top right
                          // Second triangle
        1.f, 0.3f, 0.f,   // Top right
        1.f, -0.3f, 0.f,  // Bottom right
        -1.f, -0.3f, 0.f  // Bottom left
    };

    const char *vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "out vec3 pos;"
        "void main () {"
        "     pos = vec3(vp.x,vp.y,0.5);"
        "     gl_Position = vec4(vp, 1.0);"
        "}";
    const char *vertex_shader_02 =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "uniform mat4 modelMatrix;"
        "out vec3 pos;"
        "void main () {"
        "     pos = vec3(vp.x,vp.y,1);"
        "     gl_Position = modelMatrix * vec4(vp, 1.0);"
        "}";
    const char *fragment_shader =
        "#version 330\n"
        "in vec3 pos;"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4(pos, 1.0);}";
    Scene *scene1 = new Scene();
    this->addScene(scene1);

    Model *test = new Model(points, sizeof(points), 3);
    Model *squareModel = new Model(square, sizeof(square), 6);
    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Shader *vertexShader = new Shader(vertex_shader, GL_VERTEX_SHADER);
    Shader *fragmentShader = new Shader(fragment_shader, GL_FRAGMENT_SHADER);
    Shader *vertex02 = new Shader(vertex_shader_02, GL_VERTEX_SHADER);

    ShaderProgram *shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    ShaderProgram *shaderProgram02 = new ShaderProgram(*vertex02, *fragmentShader);
    DrawableObject *triangle = new DrawableObject(test, shaderProgram);
    DrawableObject *squareObject = new DrawableObject(squareModel, shaderProgram02);
    DrawableObject *bushesObject = new DrawableObject(bush, shaderProgram02);
    squareObject->getTransformation().setRotation(90.0, glm::vec3(1.0, 1.0, 1.0));

    scene1->addObject(bushesObject);
    scene1->addObject(squareObject);
    scene1->addObject(triangle);
    float angle = 0.01f;

    
    while (!glfwWindowShouldClose(this->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        this->scenes[active]->runScene();
        glfwSwapBuffers(window);
    }
}
