#include "SceneBuilder.hpp"

SceneBuilder::SceneBuilder(Camera *camera) : camera(camera), activeSceneIndex(0)
{
    this->camera->createFlashLight();
}

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
    scene->addLight(camera->getFlashLight());
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
    PointLight *centerLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Reflector *reflector = new Reflector(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0, -1.0, 0.0), 20.0, 50.0);
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

    // scene->addLight(centerLight);
    // scene->addLight(reflector);
    lights.push_back(reflector);

    this->createScene(scene);
}
void SceneBuilder::createForest()
{
    Scene *scene = new Scene();
    DrawableObject *tree = createObject("tree", "phong");
    tree->getMaterial()->setObjectColor(glm::vec3(0, 1, 0));

    DrawableObject *bush = createObject("bush", "phong");

    DrawableObject *firefly = createObject("sphere", "phong");
    DrawableObject *firefly2 = createObject("sphere", "phong");
    firefly->createLight(glm::vec3(1.0, 0.9, 0.3), glm::vec3(0.4, 0.4, 0.3), glm::vec3(1.0, 0.7, 1.8));
    firefly->getTransformation().setScale(glm::vec3(0.1f));
    firefly->getTransformation().setPosition(glm::vec3(0.0, -2, 0));
    firefly2->createLight(glm::vec3(1.0, 0.9, 0.3), glm::vec3(0.4, 0.4, 0.3), glm::vec3(1.0, 0.7, 1.8));
    firefly2->getTransformation().setScale(glm::vec3(0.1f));
    firefly->getMaterial()->setObjectColor(glm::vec3(1, 1, 1));
    firefly->getMaterial()->setEmission(glm::vec3(1.5, 1.2, 0.5));

    firefly2->getMaterial()->setObjectColor(glm::vec3(1, 1, 1));
    firefly->getMaterial()->setObjectColor(glm::vec3(1, 1, 1));
    firefly->createRandomMovement(1.5f,1.0f);
    firefly2->createRandomMovement(1.5f,1.0f);
    std::vector<std::pair<DrawableObject *, int>> forest = {{tree, 50}, {bush, 50}};
    scene->randomForest(glm::vec3(0.0f, -1.0f, 0.0f), 5, forest);

    scene->addLight(firefly->getLight());
    scene->addLight(firefly2->getLight());
    scene->addObject(firefly);
    scene->addObject(firefly2);
    this->createScene(scene);
}
void SceneBuilder::createSunSystem()
{
    Scene *scene = new Scene();
    DrawableObject *sun = createObject("sphere", "phong");
    DrawableObject *earth = createObject("sphere", "phong");
    DrawableObject *moon = createObject("sphere", "phong");
    sun->getTransformation().setScale(glm::vec3(1.0f));
    sun->getTransformation().setPosition(glm::vec3(0.0f, 0.0f, 0.f));
    sun->createLight(glm::vec3(1), glm::vec3(1), glm::vec3(1, 0.09, 0.032));
    scene->addLight(sun->getLight());
    earth->getTransformation().setScale(glm::vec3(0.3f));
    earth->createOrbit(sun, 2.0f, 30.0f, 0.0f);
    moon->getTransformation().setScale(glm::vec3(0.05f));
    moon->createOrbit(earth, 0.5f, 60.0f, 0.0f);
    scene->addObject(sun);
    scene->addObject(moon);
    scene->addObject(earth);
    this->createScene(scene);
}
void SceneBuilder::createTestScene()
{
    Scene *scena = new Scene();
    DrawableObject *formula = createObject("formula", "phong");
    DrawableObject *house = createObject("house", "phong");
    DrawableObject *Koen = createObject("koen", "phong");
    DrawableObject *ferarri = createObject("ferarri", "phong");
    // house->getTransformation().setScale(glm::vec3(0.8f));
    house->getTransformation().setPosition(glm::vec3(-2.0, 0.0, 8.0));
    formula->getTransformation().setScale(glm::vec3(0.2f));
    Koen->getTransformation().setPosition(glm::vec3(0.0, 3.0, 0.0));
    Koen->getTransformation().setScale(glm::vec3(0.2f));
    ferarri->getTransformation().setPosition(glm::vec3(0.0, -3.0, 0.0));
    ferarri->getTransformation().setScale(glm::vec3(0.015f));

    formula->createRotation(20.f, glm::vec3(0, 1, 0), -1);
    Koen->createRotation(30.f, glm::vec3(0, 1, 0), 1);
    ferarri->createRotation(25.f, glm::vec3(0, 1, 0), 1);
    house->createOrbit(formula, 9.0, 40.0, 0.0);
    scena->addObject(formula);
    scena->addObject(house);
    scena->addObject(Koen);
    scena->addObject(ferarri);
    Directional *light = new Directional(glm::vec3(0.0f, 10.0f, -2.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    // scena->addLight(light);
    this->createScene(scena);

    Scene *scena2 = new Scene();

    DrawableObject *lostEmpire = createObject("lostEmpire", "phong");
    // lostEmpire->getMaterial()->loadTexture("../../Models/assets/Ferarri_texture.blend");
    lostEmpire->getTransformation().setPosition(glm::vec3(0, -15, 0));

    scena2->addObject(lostEmpire);
    scena2->addLight(light);
    this->createScene(scena2);
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