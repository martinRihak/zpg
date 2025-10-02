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

void App::addObjectToScene(DrawableObject* obj,int8_t id){
    this->scenes[id]->addObject(obj);
}
int8_t App::getSceneCount(){
    return this->sceneCount;
}
int8_t App::activeScene(){
    return this->active;
}
void App::createScene()
{
    this->scenes.push_back(new Scene());
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

    Model *test = new Model(points, sizeof(points), 3);
    Model *squareModel = new Model(square, sizeof(square), 6);
    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Shader *vertexShader = new Shader(vertex_shader, GL_VERTEX_SHADER);
    Shader *fragmentShader = new Shader(fragment_shader, GL_FRAGMENT_SHADER);
    Shader *vertex02 = new Shader(vertex_shader_02, GL_VERTEX_SHADER);

    ShaderProgram *shaderProgram = new ShaderProgram(*vertexShader, *fragmentShader);
    ShaderProgram *shaderProgram02 = new ShaderProgram(*vertex02, *fragmentShader);
    DrawableObject *triangle = new DrawableObject(test, shaderProgram02);
    DrawableObject *squareObject = new DrawableObject(squareModel, shaderProgram02);
    DrawableObject *bushesObject = new DrawableObject(bush, shaderProgram02);

    this->createScene();
    this->addObjectToScene(triangle,0);


    triangle->createRotarion(100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Use glfwGetTime() to compute real delta-time per frame
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(this->window))
    {
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        this->scenes[active]->render(dt);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
