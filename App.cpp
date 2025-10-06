#include "App.hpp"
#include "Controller.hpp"

App::App(int width, int height) : width(width), height(height)
{

    if (!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW3\n";
        exit(EXIT_FAILURE);
    }
    // Request a depth buffer (24 bits)
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
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
    // lazily create controller on first scene creation
    if (!this->controller)
        this->controller = new Controller();
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

    const char *vertex_shader_02 =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "uniform mat4 modelMatrix;"
        "out vec3 pos;"
        "void main () {"
        "     pos = vec3(vp.x,vp.y,vp.z);"
        "     gl_Position = modelMatrix * vec4(vp, 1.0);"
        "}";
    const char *fragment_shader =
        "#version 330\n"
        "in vec3 pos;"
        "out vec4 frag_colour;\n"
        "void main () {"
        "     frag_colour = vec4(pos, 1.0);}";
    const char *fragment_shader_red =
        "#version 330\n"
        "in vec3 pos;\n"
        "out vec4 frag_colour;\n"
        "void main () { frag_colour = vec4(1.0, 0.2, 0.2, 1.0); }";

    const char *fragment_shader_green =
        "#version 330\n"
        "in vec3 pos;\n"
        "out vec4 frag_colour;\n"
        "void main () { frag_colour = vec4(0.2, 1.0, 0.2, 1.0); }";
    Model *test = new Model(points, sizeof(points), 3);
    Model *squareModel = new Model(square, sizeof(square), 6);
    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Model *sphereModel = new Model(sphere, sizeof(sphere), 2880);
    Shader *fragmentShader = new Shader(fragment_shader, GL_FRAGMENT_SHADER);
    Shader *vertex02 = new Shader(vertex_shader_02, GL_VERTEX_SHADER);

    ShaderProgram *shaderProgram02 = new ShaderProgram(*vertex02, *fragmentShader);

    Shader *fragRed = new Shader(fragment_shader_red, GL_FRAGMENT_SHADER);
    Shader *fragGreen = new Shader(fragment_shader_green, GL_FRAGMENT_SHADER);
    ShaderProgram *redProgram = new ShaderProgram(*vertex02, *fragRed);
    ShaderProgram *greenProgram = new ShaderProgram(*vertex02, *fragGreen);
    DrawableObject *triangle = new DrawableObject(test, shaderProgram02);
    DrawableObject *bushesObject = new DrawableObject(bush, shaderProgram02);

    DrawableObject *sphere1 = new DrawableObject(sphereModel, shaderProgram02);
    DrawableObject *sphere2 = new DrawableObject(sphereModel, greenProgram);
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
    this->createScene();
    // add two spheres to scene 0 and two to scene 1 (symmetrically positioned)

    this->addObjectToScene(triangle, 0);

    this->addObjectToScene(sphere1, 1);
    this->addObjectToScene(sphere3, 1);
    this->addObjectToScene(sphere2, 1);
    this->addObjectToScene(sphere4, 1);

    // create a third scene (index 2) and populate with many objects
    this->createScene();
    std::vector<Model *> models = {
        sphereModel,
        //new Model(suziFlat, sizeof(suziFlat), 2904),
        new Model(plain, sizeof(plain), 6),
        new Model(gift, sizeof(gift), 66624),
        new Model(tree, sizeof(tree), 92814),
        new Model(bushes, sizeof(bushes), 8730)};
    int modelCount = static_cast<int>(models.size());
    int created = 0;
    for (int y = -3; y <= 3; ++y)
    {
        for (int x = -3; x <= 3; ++x)
        {
            if (created >= 25)
                break;
            Model *m = models[created % modelCount];
            ShaderProgram *sp = (created % 3 == 0) ? shaderProgram02 : (created % 3 == 1 ? redProgram : greenProgram);
            DrawableObject *obj = new DrawableObject(m, sp);
            float fx = x * 0.2f;
            float fy = y * 0.1f;
            obj->getTransformation().setPosition(glm::vec3(fx, fy, 0.0f));
            float s = 0.05f + (created % 4) * 0.03f;
            obj->getTransformation().setScale(glm::vec3(s));
            this->addObjectToScene(obj, 2);
            ++created;
        }
        if (created >= 25)
            break;
    }

    triangle->createRotarion(100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Use glfwGetTime() to compute real delta-time per frame
    double lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        if (this->controller)
            this->controller->processInput(this->window, this->scenes, this->active, this->sceneCount);

        if (!this->scenes.empty() && this->active >= 0 && this->active < this->sceneCount)
        {
            this->scenes[this->active]->render(dt);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
