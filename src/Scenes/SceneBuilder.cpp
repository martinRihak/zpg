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
    DrawableObject *triangleObj = createObject("triangle3DModel", "phong");
    triangleObj->getMaterial()->loadTexture("../src/wooden_fence.png");
    triangleObj->getTransformation().setPosition(glm::vec3(-1, 0, 0));
    DrawableObject *triangleObj2 = createObject("triangle3DModel", "phong");
    triangleObj2->getMaterial()->loadTexture("../src/grass.png");
    triangleObj2->getTransformation().setPosition(glm::vec3(1, 0, 0));
    scene->addObject(triangleObj);
    scene->addObject(triangleObj2);

    this->createScene(scene);
}

void SceneBuilder::create4Spheres()
{
    Scene *scene = new Scene;
    DrawableObject *sphere1 = createObject("sphere", "constant");
    sphere1->getMaterial()->setObjectColor(glm::vec3(1, 0, 0));

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

    scene->addLight(centerLight);
    scene->addLight(reflector);
    lights.push_back(reflector);

    this->createScene(scene);
}
void SceneBuilder::createForest()
{
    Scene *scene = new Scene();
    DrawableObject *tree = createObject("tree", "phong");
    tree->getMaterial()->setObjectColor(glm::vec3(0, 1, 0));

    DrawableObject *shrek = createObject("shrek", "phong");
    shrek->getMaterial()->loadTexture("../Models/assets/shrek/shrek.png");
    shrek->getTransformation().setPosition(glm::vec3(5, 0, 0));
    DrawableObject *fiona = createObject("fiona", "phong");
    fiona->getMaterial()->loadTexture("../Models/assets/shrek/fiona.png");
    fiona->getTransformation().setPosition(glm::vec3(7, 0, 0));
    DrawableObject *toiled = createObject("toiled", "phong");
    toiled->getMaterial()->loadTexture("../Models/assets/shrek/toiled.jpg");
    toiled->getTransformation().setPosition(glm::vec3(6, 0, -3));
    toiled->getTransformation().setRotation(45.0f, glm::vec3(0, 1, 0));

    DrawableObject *bush = createObject("bush", "phong");
    DrawableObject *plane = createObject("plane", "phong");
    plane->getMaterial()->loadTexture("../src/grass.png");
    plane->getTransformation().setPosition(glm::vec3(0, 0, 0));
    DrawableObject *plane2 = createObject("plane", "phong");
    plane2->getTransformation().setPosition(glm::vec3(20, 0, 0));
    plane2->getMaterial()->loadTexture("../src/grass.png");
    plane2->getTransformation().setPosition(glm::vec3(0, 0, 0));
    DrawableObject *firefly = createObject("sphere", "phong");
    DrawableObject *firefly2 = createObject("sphere", "phong");
    firefly->createLight(glm::vec3(1.0, 0.9, 0.3), glm::vec3(0.4, 0.4, 0.3), glm::vec3(1.0, 0.7, 1.8));
    firefly->getTransformation().setScale(glm::vec3(0.05f));
    firefly->getTransformation().setPosition(glm::vec3(0.0, -2, 0));
    firefly->getMaterial()->setObjectColor(glm::vec3(1, 1, 1));
    firefly->getMaterial()->setEmission(glm::vec3(1.5, 1.2, 0.5));

    firefly2->createLight(glm::vec3(1.0, 0.9, 0.3), glm::vec3(0.4, 0.4, 0.3), glm::vec3(1.0, 0.7, 1.8));
    firefly2->getTransformation().setScale(glm::vec3(0.05f));
    firefly2->getMaterial()->setObjectColor(glm::vec3(1, 1, 1));
    firefly2->getMaterial()->setEmission(glm::vec3(1.5, 1.2, 0.5));
    firefly->createRandomMovement(0.5f, 2.f);
    firefly2->createRandomMovement(0.5f, 2.0f);
    std::vector<std::pair<DrawableObject *, int>> forest = {{tree, 50}, {bush, 50}};
    scene->randomForest(glm::vec3(-5.0f, 0.0f, 0.0f), 10, forest);
    scene->addObject(plane);
    scene->addObject(shrek);
    scene->addObject(fiona);
    scene->addObject(toiled);
    scene->addObject(plane2);
    scene->addLight(firefly->getLight());
    scene->addLight(firefly2->getLight());
    scene->addObject(firefly);
    scene->addObject(firefly2);
    this->createScene(scene);
}
void SceneBuilder::createSunSystem()
{
    Scene *scene = new Scene();

    // Slunce
    DrawableObject *sun = createObject("sphere", "phong");
    sun->getMaterial()->loadTexture("../Models/NASA/2k_sun.jpg");
    sun->getTransformation().setScale(glm::vec3(1.0f));
    sun->getTransformation().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sun->createRotation(5.0f, glm::vec3(0, 1, 0), 1);                                   
    sun->createLight(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.f, 0.00001f)); 
    scene->addLight(sun->getLight());

    // Merkur (bez měsíců)
    DrawableObject *mercury = createObject("sphere", "phong");
    mercury->getMaterial()->loadTexture("../Models/NASA/2k_mercury.jpg");
    mercury->getTransformation().setScale(glm::vec3(0.05f)); // Zvětšeno pro viditelnost (real ~0.0035)
    mercury->createOrbit(sun, 1.0f, 60.0f, 0.0f);            // Blízko Slunce, rychlý oběh
    mercury->createRotation(0.5f, glm::vec3(0, 1, 0), 1);    // Pomalá rotace

    // Venuše (bez měsíců, retrográdní rotace)
    DrawableObject *venus = createObject("sphere", "phong");
    venus->getMaterial()->loadTexture("../Models/NASA/2k_venus_atmosphere.jpg");
    venus->getTransformation().setScale(glm::vec3(0.1f));   // Zvětšeno pro viditelnost (real ~0.0087)
    venus->createOrbit(sun, 2.0f, 40.0f, 90.0f);            // Počáteční úhel pro rozložení
    venus->createRotation(-0.1f, glm::vec3(0, 1, 0), 1);    // Retrográdní (záporná rychlost)

    // Země a její Měsíc
    DrawableObject *earth = createObject("sphere", "phong");
    earth->getMaterial()->loadTexture("../Models/NASA/2k_earth_daymap.jpg");
    earth->getTransformation().setScale(glm::vec3(0.1f));   // Zvětšeno pro viditelnost (real ~0.0092)
    earth->createOrbit(sun, 3.0f, 30.0f, 180.0f);
    earth->createRotation(30.0f, glm::vec3(0, 1, 0), 1); 

    DrawableObject *moon = createObject("sphere", "phong");
    moon->getMaterial()->loadTexture("../Models/NASA/2k_moon.jpg");
    moon->getTransformation().setScale(glm::vec3(0.03f));   // Zvětšeno relativně k Zemi
    moon->createOrbit(earth, 0.2f, 60.0f, 0.0f);            // Mírně menší radius pro lepší viditelnost
    moon->createRotation(10.0f, glm::vec3(0, 1, 0), 1);

    // Mars (bez měsíců)
    DrawableObject *mars = createObject("sphere", "phong");
    mars->getMaterial()->loadTexture("../Models/NASA/2k_mars.jpg");
    mars->getTransformation().setScale(glm::vec3(0.07f));   // Zvětšeno pro viditelnost (real ~0.0049)
    mars->createOrbit(sun, 4.0f, 20.0f, 270.0f);
    mars->createRotation(25.0f, glm::vec3(0, 1, 0), 1);

    // Jupiter (bez měsíců)
    DrawableObject *jupiter = createObject("sphere", "phong");
    jupiter->getMaterial()->loadTexture("../Models/NASA/2k_jupiter.jpg");
    jupiter->getTransformation().setScale(glm::vec3(0.4f)); // Relativně velký (real ~0.1)
    jupiter->createOrbit(sun, 20.0f, 10.0f, 45.0f);
    jupiter->createRotation(50.0f, glm::vec3(0, 1, 0), 1); 

    // Saturn (bez měsíců)
    DrawableObject *saturn = createObject("sphere", "phong");
    saturn->getMaterial()->loadTexture("../Models/NASA/2k_saturn.jpg");
    saturn->getTransformation().setScale(glm::vec3(0.3f));  // Relativně velký (real ~0.084)
    saturn->createOrbit(sun, 35.0f, 5.0f, 135.0f);
    saturn->createRotation(40.0f, glm::vec3(0, 1, 0), 1);

    // Uranus (bez měsíců, retrográdní rotace)
    DrawableObject *uranus = createObject("sphere", "phong");
    uranus->getMaterial()->loadTexture("../Models/NASA/2k_uranus.jpg");
    uranus->getTransformation().setScale(glm::vec3(0.2f));  // Zvětšeno pro viditelnost (real ~0.036)
    uranus->createOrbit(sun, 50.0f, 3.0f, 225.0f);          // Větší vzdálenost, pomalejší oběh
    uranus->createRotation(-35.0f, glm::vec3(0, 1, 0), 1);  // Retrográdní rotace

    // Neptun (bez měsíců)
    DrawableObject *neptune = createObject("sphere", "phong");
    neptune->getMaterial()->loadTexture("../Models/NASA/2k_neptune.jpg");
    neptune->getTransformation().setScale(glm::vec3(0.2f)); // Zvětšeno pro viditelnost (real ~0.035)
    neptune->createOrbit(sun, 60.0f, 2.0f, 315.0f);         // Největší vzdálenost, nejpomalejší oběh
    neptune->createRotation(40.0f, glm::vec3(0, 1, 0), 1);

    // Přidání všech objektů do scény
    scene->addObject(sun);
    scene->addObject(mercury);
    scene->addObject(venus);
    scene->addObject(earth);
    scene->addObject(moon);
    scene->addObject(mars);
    scene->addObject(jupiter);
    scene->addObject(saturn);
    scene->addObject(uranus);
    scene->addObject(neptune);
  
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
    ferarri->getMaterial()->setObjectColor(glm::vec3(0, 1, 0));
    Koen->getMaterial()->setObjectColor(glm::vec3(0, 0, 1));
    formula->getMaterial()->setObjectColor(glm::vec3(1, 0, 0));
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
    scena->addLight(light);
    this->createScene(scena);
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