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
    glfwSetWindowUserPointer(this->window, this);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwGetFramebufferSize(window, &width, &height);
    glEnable(GL_DEPTH_TEST);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
    this->camera = new Camera();
    camera->setAspectRatio(ratio);
    this->controller = new Controller();
    this->callbackHandler = new CallbackHandler(this->window, this);
}
App::~App()
{
    for (auto *a : this->scenes)
    {
        delete a;
    }
    if (this->controller)
        delete this->controller;
    if (this->callbackHandler)
    {
        delete this->callbackHandler;
    }
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

    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Model *sphereModel = new Model(sphere, sizeof(sphere), 2880);
    Model *treeModel = new Model(tree, sizeof(tree), 92814);
    Model *triangleModel = new Model(triangle, sizeof(triangle), 3);

    // Initialize shaders
    Shader *lambertFrag = new Shader("../shaders/Lambert.frag", GL_FRAGMENT_SHADER);
    Shader *constantFrag = new Shader("../shaders/Constant.frag", GL_FRAGMENT_SHADER);
    Shader *phongFrag = new Shader("../shaders/Phong.frag", GL_FRAGMENT_SHADER);
    Shader *blinnFrag = new Shader("../shaders/Blinn.frag", GL_FRAGMENT_SHADER);
    Shader *vertex02 = new Shader("../shaders/vert.vert", GL_VERTEX_SHADER);

    ShaderProgram *lambertShader = new ShaderProgram(*vertex02, *lambertFrag, this->camera);
    ShaderProgram *constantShader = new ShaderProgram(*vertex02, *constantFrag, this->camera);
    ShaderProgram *phongShader = new ShaderProgram(*vertex02, *phongFrag, this->camera);
    ShaderProgram *blinnShader = new ShaderProgram(*vertex02, *blinnFrag, this->camera);

    SceneBuilder builder(this->camera);
    builder.registerModel("triangle", triangleModel);
    builder.registerModel("bush", bush);
    builder.registerModel("sphere", sphereModel);
    builder.registerModel("tree", treeModel);
    builder.registerShader("lambert", lambertShader);
    builder.registerShader("constant", constantShader);
    builder.registerShader("phong", phongShader);
    builder.registerShader("blinn", blinnShader);

    builder.createTriangle();
    builder.create4Spheres();
    builder.createForest();
    builder.createSunSystem();
    double lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        if (this->controller)
        {
            this->controller->processInput(this->window, builder.getSceneCount(), this->camera, dt);
            builder.setActiveSceneIndex(this->controller->getActiveScene()); // Update active scene based on controller
        }

        Scene *activeScene = builder.getScene(builder.getActiveSceneIndex());
        if (activeScene)
        {
            activeScene->render(dt);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
