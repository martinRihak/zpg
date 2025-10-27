#include "SceneBuilder.hpp"

SceneBuilder::SceneBuilder(Camera *camera) : camera(camera), activeSceneIndex(0) {}

SceneBuilder::~SceneBuilder()
{
}

void SceneBuilder::registerModel(const std::string &name, Model *model)
{
    models[name] = model;
}

void SceneBuilder::registerShader(const std::string &name, ShaderProgram *shader)
{
    this->camera->attach(shader);
    shaders[name] = shader;
}
void SceneBuilder::createScene(Scene *scene)
{
    this->scenes.push_back(scene);
}
DrawableObject *SceneBuilder::createObject(const std::string &modelName, const std::string &shaderName)
{
    auto modelIt = models.find(modelName);
    auto shaderIt = shaders.find(shaderName);
    if (modelIt != models.end() && shaderIt != shaders.end())
    {
        return new DrawableObject(modelIt->second, shaderIt->second);
    }
    return nullptr;
}
void SceneBuilder::createTriangle()
{
    Scene *scene = new Scene();
    DrawableObject *triangle = createObject("triangle", "phong");
    scene->addObject(triangle);
    this->createScene(scene);
}

void SceneBuilder::create4Spheres()
{
    Scene *scene = new Scene;
    DrawableObject *sphere1 = createObject("sphere", "phong");
    DrawableObject *sphere2 = createObject("sphere", "phong");
    DrawableObject *sphere3 = createObject("sphere", "phong");
    DrawableObject *sphere4 = createObject("sphere", "phong");
    PointLight *centerLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    //Light *leftLight = new Light(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    lights.push_back(centerLight);
    sphere1->getTransformation().setPosition(glm::vec3(0.7f, 0.0f, 0.0f));
    sphere1->getTransformation().setScale(glm::vec3(0.3f));
    sphere2->getTransformation().setPosition(glm::vec3(-0.7f, 0.0f, 0.0f));
    sphere2->getTransformation().setScale(glm::vec3(0.3f));
    sphere3->getTransformation().setPosition(glm::vec3(0.0f, -0.7f, 0.0f));
    sphere3->getTransformation().setScale(glm::vec3(0.3f));
    sphere4->getTransformation().setPosition(glm::vec3(0.0f, 0.7f, 0.0f));
    sphere4->getTransformation().setScale(glm::vec3(0.3f));

    scene->addObject(sphere1);
    scene->addObject(sphere2);
    scene->addObject(sphere3);
    scene->addObject(sphere4);

    scene->addLight(centerLight);
   // scene->addLight(leftLight);

    this->createScene(scene);
}
void SceneBuilder::createForest()
{
    Scene *scene = new Scene();
    DrawableObject *tree = createObject("tree", "phong");
    DrawableObject *bush = createObject("bush", "phong");
    PointLight* firefly = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.9f,0.0f), glm::vec3(1.0f, 0.9f, 0.0f),glm::vec3(1.0f, 0.09f, 0.44f));
    PointLight* firefly2 = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.9f,0.0f), glm::vec3(1.0f, 0.9f, 0.0f),glm::vec3(1.0f, 0.09f, 0.44f));
    firefly->createRandomMovement(0.1f,14.5f);
    firefly2->createRandomMovement(0.1f,14.5f);
    firefly2->getTransformation().setScale(glm::vec3(0.2f));
    firefly->getTransformation().setScale(glm::vec3(0.2f));
    std::vector<std::pair<DrawableObject *, int>> forest = {{tree, 50}, {bush, 50}};
    scene->randomForest(glm::vec3(0.0f, -1.0f, 0.0f), 5, forest);

    scene->addLight(firefly);
    scene->addLight(firefly2);
    this->createScene(scene);
}
void SceneBuilder::createSunSystem()
{
    Scene *scene = new Scene();
    DrawableObject *sun = createObject("sphere", "phong");
    DrawableObject *earth = createObject("sphere", "phong");
    DrawableObject *moon = createObject("sphere", "phong");
    sun->getTransformation().setScale(glm::vec3(1.0f));
    sun->getTransformation().setPosition(glm::vec3(0.0f,0.0f,0.f));
    earth->getTransformation().setScale(glm::vec3(0.3f));
    earth->createOrbit(sun, 2.0f, 30.0f, 0.0f);
    moon->getTransformation().setScale(glm::vec3(0.05f));
    moon->createOrbit(earth, 0.5f, 60.0f, 0.0f);
    scene->addLight(this->lights[0]);
    scene->addObject(sun);
    scene->addObject(moon);
    scene->addObject(earth);
    this->createScene(scene);
}
Scene *SceneBuilder::getScene(int8_t index) const
{
    if (index >= 0 && index < scenes.size())
    {
        return scenes[index];
    }
    return nullptr;
}

int8_t SceneBuilder::getSceneCount() const
{
    return static_cast<int8_t>(scenes.size());
}

int8_t SceneBuilder::getActiveSceneIndex() const
{
    return activeSceneIndex;
}

void SceneBuilder::setActiveSceneIndex(int8_t index)
{
    if (index >= 0 && index < scenes.size())
    {
        activeSceneIndex = index;
    }
}