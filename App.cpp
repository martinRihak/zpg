#include "App.hpp"
#include "Controller.hpp"

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

    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    glEnable(GL_DEPTH_TEST);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
    this->camera = new Camera();
}
App::~App()
{
    for (auto *a : this->scenes)
    {
        delete a;
    }
    if (this->controller)
        delete this->controller;
    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void App::addObjectToScene(DrawableObject *obj, int8_t id)
{
    this->scenes[id]->addObject(obj);
}
int8_t App::getSceneCount()
{
    return this->sceneCount;
}
int8_t App::activeScene()
{
    return this->active;
}
void App::createScene()
{
    this->scenes.push_back(new Scene());
    this->sceneCount = static_cast<int8_t>(this->scenes.size());
    if (!this->controller)
        this->controller = new Controller();
}

void App::addShaderProgram(ShaderProgram *program)
{
    this->camera->attach(program);
}
void App::run()
{
    float triangle[] = {

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top vertex
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // Bottom left
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


    Model *squareModel = new Model(square, sizeof(square), 6);
    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Model *sphereModel = new Model(sphere, sizeof(sphere), 2880);
    Model *treeModel = new Model(tree, sizeof(tree), 92814);
    Model *triangleModel = new Model(triangle, sizeof(triangle), 3);
    Shader *fragmentShader = new Shader();
    Shader *vertex02 = new Shader();
    vertex02->createShaderFromFile(GL_VERTEX_SHADER,"../shaders/vert.vert");
    fragmentShader->createShaderFromFile(GL_FRAGMENT_SHADER,"../shaders/Lambert.frag");

    ShaderProgram *shaderProgram02 = new ShaderProgram(*vertex02, *fragmentShader, this->camera);
    this->addShaderProgram(shaderProgram02);

    DrawableObject *triangleObject = new DrawableObject(triangleModel, shaderProgram02);
    DrawableObject *bushesObject = new DrawableObject(bush, shaderProgram02);
    DrawableObject *treeObject = new DrawableObject(treeModel, shaderProgram02);
    DrawableObject *sphere1 = new DrawableObject(sphereModel, shaderProgram02);
    DrawableObject *sphere2 = new DrawableObject(sphereModel, shaderProgram02);
    DrawableObject *sphere3 = new DrawableObject(sphereModel, shaderProgram02);
    DrawableObject *sphere4 = new DrawableObject(sphereModel, shaderProgram02);

    sphere1->getTransformation().setPosition(glm::vec3(0.7f, 0.0f, 0.0f));
    sphere1->getTransformation().setScale(glm::vec3(0.3f));

    sphere2->getTransformation().setPosition(glm::vec3(-0.7f, 0.0f, 0.0f));
    sphere2->getTransformation().setScale(glm::vec3(0.3f));

    sphere3->getTransformation().setPosition(glm::vec3(0.0f, 0.7f, 0.0f));
    sphere3->getTransformation().setScale(glm::vec3(0.3f));

    sphere4->getTransformation().setPosition(glm::vec3(0.0f, -0.7f, 0.0f));
    sphere4->getTransformation().setScale(glm::vec3(0.3f));
    this->createScene();
    this->addObjectToScene(triangleObject, 0);
    this->createScene();
    this->addObjectToScene(sphere1, 1);
    this->addObjectToScene(sphere3, 1);
    this->addObjectToScene(sphere2, 1);
    this->addObjectToScene(sphere4, 1);
    std::vector<std::pair<DrawableObject *, int>> forest = {{treeObject, 50}, {bushesObject, 50}};

    this->createScene();
    this->scenes[2]->randomForest(glm::vec3(0.0f, -1.0f, 0.0f), 5, forest);
    double lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        if (this->controller)
            this->controller->processInput(this->window, this->scenes, this->active, this->sceneCount, this->camera, dt);

        if (!this->scenes.empty() && this->active >= 0 && this->active < this->sceneCount)
        {
            this->scenes[this->active]->render(dt);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
