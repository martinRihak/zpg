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
    this->callbackHandler = new CallbackHandler(this->window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}
App::~App()
{
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

void App::createScenes()
{
  
float triangle3D[] = {
0.000000f, -0.500000f, 0.500000f, -0.872900f, 0.218200f, 0.436400f, 0.836598f, 0.477063f,
0.000000f, 0.500000f, 0.000000f, -0.872900f, 0.218200f, 0.436400f, 0.399527f, 0.286309f,
-0.500000f, -0.500000f, -0.500000f, -0.872900f, 0.218200f, 0.436400f, 0.836598f, 0.000179f,
-0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.381686f, 0.999821f,
0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.000179f, 0.809067f,
0.000000f, -0.500000f, 0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.381686f, 0.522937f,
0.500000f, -0.500000f, -0.500000f, 0.872900f, 0.218200f, 0.436400f, 0.399169f, 0.000179f,
0.000000f, 0.500000f, 0.000000f, 0.872900f, 0.218200f, 0.436400f, 0.399169f, 0.522579f,
0.000000f, -0.500000f, 0.500000f, 0.872900f, 0.218200f, 0.436400f, 0.000179f, 0.261379f,
-0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.447200f, -0.894400f, 0.788901f, 0.477421f,
0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.447200f, -0.894400f, 0.788901f, 0.999821f,
0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.447200f, -0.894400f, 0.399527f, 0.651554f
};

    //Model *model = new Model(points, sizeof(points), 12, true);
    Model *bush = new Model(bushes, sizeof(bushes), 8730);
    Model *sphereModel = new Model(sphere, sizeof(sphere), 2880);
    Model *treeModel = new Model(tree, sizeof(tree), 92814);
    Model *triangleModel = new Model(triangle, sizeof(triangle), 3);
    Model *triandel3DModel = new Model(triangle3D, sizeof(triangle3D), 12,true);
    Model *formula = new Model("../Models/assets/formula1.obj");
    Model *house = new Model("../Models/assets/house.obj");
    Model *Koen = new Model("../Models/assets/Koenigsegg.obj");
    Model *Ferarri = new Model("../Models/assets/Humvee.obj");
    Model *lostEmpire = new Model("../Models/assets/lost_empire.obj");
    Model *planeModel = new Model(plane, sizeof(plane), 6,true);
    Model* shrekModel = new Model("../Models/assets/shrek/shrek.obj");

    // Initialize shaders
    Shader *lambertFrag = new Shader("../shaders/Lambert.frag", GL_FRAGMENT_SHADER);
    Shader *constantFrag = new Shader("../shaders/Constant.frag", GL_FRAGMENT_SHADER);
    Shader *phongFrag = new Shader("../shaders/Phong.frag", GL_FRAGMENT_SHADER);
    Shader *blinnFrag = new Shader("../shaders/Blinn.frag", GL_FRAGMENT_SHADER);
    Shader *vertex02 = new Shader("../shaders/vert.vert", GL_VERTEX_SHADER);
    Shader *assimpVertex = new Shader("../shaders/Assimp.vert", GL_VERTEX_SHADER);
    Shader *TextureVertex = new Shader("../shaders/Texture.vert", GL_VERTEX_SHADER);
    
    ShaderProgram *lambertShader = new ShaderProgram(*vertex02, *lambertFrag);
    ShaderProgram *constantShader = new ShaderProgram(*vertex02, *constantFrag);
    ShaderProgram *phongShader = new ShaderProgram(*vertex02, *phongFrag);
    ShaderProgram *blinnShader = new ShaderProgram(*vertex02, *blinnFrag);

    ShaderProgram *TextureShader = new ShaderProgram(*TextureVertex, *phongFrag);
    ShaderProgram *assimpPhong = new ShaderProgram(*assimpVertex, *phongFrag);

    this->builder = new SceneBuilder(this->camera);
    builder->registerModel("triangle", triangleModel);
    builder->registerModel("triangle3DModel", triandel3DModel);
    builder->registerModel("plane", planeModel);
    builder->registerModel("ferarri", Ferarri);
    builder->registerModel("lostEmpire", lostEmpire);
    builder->registerModel("bush", bush);
    builder->registerModel("sphere", sphereModel);
    builder->registerModel("tree", treeModel);
    builder->registerModel("formula", formula);
    builder->registerModel("house", house);
    builder->registerModel("koen", Koen);

    builder->registerModel("shrek", shrekModel);

    builder->registerShader("lambert", lambertShader);
    builder->registerShader("constant", constantShader);
    builder->registerShader("phong", phongShader);
    builder->registerShader("blinn", blinnShader);
    builder->registerShader("assimpPhong", assimpPhong);
    builder->registerShader("TexturePhong", TextureShader);

    builder->createTriangle();
    builder->create4Spheres();
    builder->createForest();
    builder->createSunSystem();
    //builder->createTestScene();
    builder->createShrek();
}
void App::run()
{
    createScenes();

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
            this->controller->processInput(this->window, builder->getSceneCount(), this->camera, dt);
            builder->setActiveSceneIndex(this->controller->getActiveScene()); // Update active scene based on controller
        }

        Scene *activeScene = builder->getScene(builder->getActiveSceneIndex());
        if (activeScene)
        {
            activeScene->render(dt);
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}
