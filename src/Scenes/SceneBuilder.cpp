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
void SceneBuilder::createSkyBox()
{

    std::vector<std::string> skyboxSky = {
        "../Models/NASA/cube/stars_posx.jpg",
        "../Models/NASA/cube/stars_negx.jpg",
        "../Models/NASA/cube/stars_posy.jpg",
        "../Models/NASA/cube/stars_negy.jpg",
        "../Models/NASA/cube/stars_posz.jpg",
        "../Models/NASA/cube/stars_negz.jpg"};
    DrawableObject *cube02 = createObject("cube", "skyboxShader");
    cube02->getMaterial()->setFaces(skyboxSky);
    cube02->getTransformation().setScale(glm::vec3(50));
    cube02->getMaterial()->loadCubeMap();
    this->skybox.push_back(cube02);
}
void SceneBuilder::createTriangle()
{
    std::vector<glm::vec3> raceTrackPoints = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(20.0f, 0.5f, 5.0f),
        glm::vec3(30.0f, 1.0f, 0.0f),
    };
    Scene *scene = new Scene();
    DrawableObject *rocket = createObject("rocket", "phong");
    rocket->createBezier(raceTrackPoints, 0.05f, true);

    DrawableObject *triangleObj = createObject("tree", "phong");
    triangleObj->getMaterial()->loadTexture("../src/wooden_fence.png");
    triangleObj->getTransformation().setUseCustom(true);
    triangleObj->getTransformation().setPosition(glm::vec3(-1, 0, 0));
    DrawableObject *triangleObj2 = createObject("tree", "phong");
    triangleObj2->getMaterial()->loadTexture("../src/grass.png");
    triangleObj2->getTransformation().setPosition(glm::vec3(1, 0, 0));
    triangleObj2->getTransformation().setScale(glm::vec3(0.2f));
    triangleObj2->addAnimator(new BasicBezier(
        glm::mat4(glm::vec4(-1.0, 3.0, -3.0, 1.0),
                  glm::vec4(3.0, -6.0, 3.0, 0),
                  glm::vec4(-3.0, 3.0, 0, 0),
                  glm::vec4(1, 0, 0, 0)),
        glm::mat4x3(glm::vec3(-1, 0, 0),
                    glm::vec3(0, 5, 0),
                    glm::vec3(0, -5, 0),
                    glm::vec3(3, 3, 0)),
        0.5));
    Directional *light = new Directional(glm::vec3(0.0f, 20.0f, -2.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    scene->addLight(light);
    scene->addObject(triangleObj);
    scene->addObject(triangleObj2);
    scene->addObject(rocket);
    scene->addObject(skybox[0]);
    this->createScene(scene);
}

void SceneBuilder::create4Spheres()
{
    Scene *scene = new Scene;
    DrawableObject *sphere1 = createObject("sphere", "constant");
    sphere1->getMaterial()->setObjectColor(glm::vec3(1, 0, 0));

    DrawableObject *sphere2 = createObject("sphere", "lambert");
    DrawableObject *sphere3 = createObject("sphere", "phong");
    DrawableObject *sphere4 = createObject("sphere", "blinn");
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
    std::vector<std::string> skyboxFaces = {
        "../Models/cubeMap/posx.jpg",
        "../Models/cubeMap/negx.jpg",
        "../Models/cubeMap/posy.jpg",
        "../Models/cubeMap/negy.jpg",
        "../Models/cubeMap/posz.jpg",
        "../Models/cubeMap/negz.jpg"};
    DrawableObject *cube = createObject("cube", "skyboxShader");
    cube->getTransformation().setScale(glm::vec3(50));
    cube->getMaterial()->setTime(0.2f);
    cube->getMaterial()->setFaces(skyboxFaces);
    cube->getMaterial()->loadCubeMap();

    Scene *scene = new Scene();
    DrawableObject *tree = createObject("tree", "phong");
    tree->getMaterial()->setObjectColor(glm::vec3(0, 1, 0));

    DrawableObject *shrek = createObject("shrek", "phong");
    shrek->getTransformation().setPosition(glm::vec3(1, 0, 0));
    shrek->getTransformation().setRotation(-45.0f, glm::vec3(0, 1, 0));
    DrawableObject *fiona = createObject("fiona", "phong");
    fiona->getTransformation().setPosition(glm::vec3(-1, 0, 0));
    fiona->getTransformation().setRotation(45.0f, glm::vec3(0, 1, 0));
    DrawableObject *toiled = createObject("toiled", "phong");
    toiled->getTransformation().setPosition(glm::vec3(-6, 0, -4));
    toiled->getTransformation().setRotation(45.0f, glm::vec3(0, 1, 0));

    DrawableObject *house = createObject("shrekHouse", "phong");
    house->getTransformation().setPosition(glm::vec3(0, 0, -7));
    house->getTransformation().setRotation(-45.0f, glm::vec3(0, 1, 0));

    DrawableObject *bush = createObject("bush", "phong");
    DrawableObject *plane = createObject("plane", "phong");
    plane->getMaterial()->loadTexture("../src/grass.png");
    plane->getTransformation().setPosition(glm::vec3(0, 0, 0));
    plane->getTransformation().setScale(glm::vec3(3.f));
    DrawableObject *plane2 = createObject("plane", "phong");
    plane2->getTransformation().setPosition(glm::vec3(20, 0, 0));
    plane2->getMaterial()->loadTexture("../src/grass.png");
    plane2->getTransformation().setPosition(glm::vec3(0, 0, 0));
    plane2->getTransformation().setScale(glm::vec3(3.f));
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
    std::vector<std::pair<DrawableObject *, int>> forest = {{tree, 50}, {bush, 20}};
    scene->randomForest(glm::vec3(12.0f, 0.0f, 0.0f), 10, forest);
    scene->randomForest(glm::vec3(-12.0f, 0.0f, 0.0f), 10, forest);
    scene->addObject(plane);
    scene->addObject(cube);
    scene->addObject(shrek);
    scene->addObject(fiona);
    scene->addObject(toiled);
    scene->addObject(plane2);
    scene->addLight(firefly->getLight());
    scene->addLight(firefly2->getLight());
    scene->addObject(firefly);
    scene->addObject(firefly2);
    scene->addObject(house);
    this->createScene(scene);
}
void SceneBuilder::createSunSystem()
{

    Scene *scene = new Scene();
    // Slunce
    DrawableObject *sun = createObject("sphereOBJ", "constant");
    sun->getMaterial()->loadTexture("../Models/NASA/2k_sun.jpg");
    sun->getTransformation().setScale(glm::vec3(1.0f));
    sun->getTransformation().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sun->createRotation(5.0f, glm::vec3(0, 1, 0), 1);
    sun->createLight(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.f, 0.00001f));
    scene->addLight(sun->getLight());

    DrawableObject *mercury = createObject("sphereOBJ", "phong");
    mercury->getMaterial()->loadTexture("../Models/NASA/2k_mercury.jpg");
    mercury->getTransformation().setScale(glm::vec3(0.05f));
    mercury->createOrbit(sun, 1.0f, 60.0f, 0.0f);

    DrawableObject *venus = createObject("sphereOBJ", "phong");
    venus->getMaterial()->loadTexture("../Models/NASA/2k_venus_atmosphere.jpg");
    venus->getTransformation().setScale(glm::vec3(0.1f));
    venus->createOrbit(sun, 2.0f, 40.0f, 90.0f);

    DrawableObject *earth = createObject("sphereOBJ", "phong");
    earth->getMaterial()->loadTexture("../Models/NASA/2k_earth_daymap.jpg");
    earth->getTransformation().setScale(glm::vec3(0.1f));
    earth->createOrbit(sun, 3.0f, 30.0f, 180.0f);

    DrawableObject *moon = createObject("sphereOBJ", "phong");
    moon->getMaterial()->loadTexture("../Models/NASA/2k_moon.jpg");
    moon->getTransformation().setScale(glm::vec3(0.03f));
    moon->createOrbit(earth, 0.2f, 60.0f, 0.0f);

    DrawableObject *mars = createObject("sphereOBJ", "phong");
    mars->getMaterial()->loadTexture("../Models/NASA/2k_mars.jpg");
    mars->getTransformation().setScale(glm::vec3(0.07f));
    mars->createOrbit(sun, 4.0f, 20.0f, 270.0f);

    DrawableObject *jupiter = createObject("sphereOBJ", "phong");
    jupiter->getMaterial()->loadTexture("../Models/NASA/2k_jupiter.jpg");
    jupiter->getTransformation().setScale(glm::vec3(0.4f));
    jupiter->createOrbit(sun, 20.0f, 10.0f, 45.0f);

    DrawableObject *saturn = createObject("sphereOBJ", "phong");
    saturn->getMaterial()->loadTexture("../Models/NASA/2k_saturn.jpg");
    saturn->getTransformation().setScale(glm::vec3(0.3f));
    saturn->createOrbit(sun, 35.0f, 5.0f, 135.0f);

    DrawableObject *uranus = createObject("sphereOBJ", "phong");
    uranus->getMaterial()->loadTexture("../Models/NASA/2k_uranus.jpg");
    uranus->getTransformation().setScale(glm::vec3(0.2f));
    uranus->createOrbit(sun, 50.0f, 3.0f, 225.0f);

    DrawableObject *neptune = createObject("sphereOBJ", "phong");
    neptune->getMaterial()->loadTexture("../Models/NASA/2k_neptune.jpg");
    neptune->getTransformation().setScale(glm::vec3(0.2f));
    neptune->createOrbit(sun, 60.0f, 2.0f, 315.0f);
    scene->addObject(skybox[0]);
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
void SceneBuilder::createRaceTrack()
{
    std::vector<glm::vec3> racePoints = {
        glm::vec3(-1.195549f, -0.002268f, 0.320951f), // Anchor 0
        glm::vec3(-3.182554f, -0.003398f, 0.570556f), // Control 1
        glm::vec3(-3.864220f, -0.004058f, 0.682624f), // Control 2

        // Segment 1
        glm::vec3(-9.612954f, -0.005435f, 2.007441f),  // Anchor 1
        glm::vec3(-11.183517f, -0.005883f, 2.632321f), // Control 1
        glm::vec3(-11.313234f, -0.005546f, 3.004949f), // Control 2

        // Segment 2
        glm::vec3(-15.198067f, -0.006311f, 4.427852f), // Anchor 2
        glm::vec3(-17.045133f, -0.006960f, 4.916194f), // Control 1
        glm::vec3(-17.548966f, -0.007856f, 4.959759f), // Control 2

        // Segment 3
        glm::vec3(-21.406281f, -0.009493f, 3.930578f), // Anchor 3
        glm::vec3(-23.440138f, -0.010426f, 3.232136f), // Control 1
        glm::vec3(-23.969256f, -0.008604f, 1.485010f), // Control 2

        // Segment 4
        glm::vec3(-27.449684f, -0.005776f, -3.314631f), // Anchor 4
        glm::vec3(-28.433953f, -0.005094f, -5.258684f), // Control 1
        glm::vec3(-28.506816f, -0.005103f, -6.420078f), // Control 2

        // Segment 5
        glm::vec3(-26.468117f, -0.001585f, -9.288795f),  // Anchor 5
        glm::vec3(-25.710114f, -0.000301f, -10.624357f), // Control 1
        glm::vec3(-25.001798f, -0.001033f, -11.424430f), // Control 2

        // Segment 6
        glm::vec3(-21.363127f, -0.002899f, -13.505445f), // Anchor 6
        glm::vec3(-19.657422f, -0.003420f, -14.492901f), // Control 1
        glm::vec3(-17.878059f, -0.004186f, -15.363656f), // Control 2

        // Segment 7
        glm::vec3(-10.896156f, -0.005128f, -17.841543f), // Anchor 7
        glm::vec3(-8.778522f, -0.005659f, -18.617341f),  // Control 1
        glm::vec3(-7.535917f, -0.006718f, -18.451695f),  // Control 2

        // Segment 8
        glm::vec3(-4.874798f, -0.008116f, -14.296818f), // Anchor 8
        glm::vec3(-3.605691f, -0.008731f, -12.576759f), // Control 1
        glm::vec3(-3.688160f, -0.008547f, -11.170652f), // Control 2

        // Segment 9
        glm::vec3(-8.749309f, -0.008124f, -8.387122f),  // Anchor 9
        glm::vec3(-10.488372f, -0.008076f, -7.483707f), // Control 1
        glm::vec3(-11.973001f, -0.007629f, -7.276085f), // Control 2

        // Segment 10
        glm::vec3(-18.863472f, -0.005676f, -7.678588f), // Anchor 10
        glm::vec3(-20.465481f, -0.005070f, -7.284795f), // Control 1
        glm::vec3(-20.812141f, -0.004871f, -6.084391f), // Control 2

        // Segment 11
        glm::vec3(-17.900444f, -0.006347f, -1.967047f), // Anchor 11
        glm::vec3(-16.806005f, -0.006868f, -0.459519f), // Control 1
        glm::vec3(-15.145297f, -0.007526f, -0.104377f), // Control 2

        // Segment 12
        glm::vec3(-8.060884f, -0.009716f, -2.605188f), // Anchor 12
        glm::vec3(-5.601828f, -0.010596f, -3.354056f), // Control 1
        glm::vec3(-3.102038f, -0.011248f, -4.810794f), // Control 2

        // Segment 13
        glm::vec3(4.039268f, -0.011220f, -11.998540f), // Anchor 13
        glm::vec3(5.713342f, -0.011871f, -13.864799f), // Control 1
        glm::vec3(6.101143f, -0.012572f, -15.032980f), // Control 2

        // Segment 14
        glm::vec3(12.575954f, -0.015718f, -16.981590f), // Anchor 14
        glm::vec3(15.164223f, -0.017100f, -17.873307f), // Control 1
        glm::vec3(17.589561f, -0.018536f, -17.878267f), // Control 2

        // Segment 15
        glm::vec3(26.973827f, -0.022296f, -16.160179f), // Anchor 15
        glm::vec3(28.978656f, -0.023236f, -15.596426f), // Control 1
        glm::vec3(29.599311f, -0.016819f, -13.124381f), // Control 2

        // Segment 16
        glm::vec3(30.002054f, -0.011156f, -6.072968f), // Anchor 16
        glm::vec3(30.337064f, -0.009565f, -2.781806f), // Control 1
        glm::vec3(29.242834f, -0.008696f, -1.506426f), // Control 2

        // Segment 17
        glm::vec3(25.608234f, -0.008326f, 0.523492f), // Anchor 17
        glm::vec3(23.668748f, -0.007925f, 1.701527f), // Control 1
        glm::vec3(21.670870f, -0.007217f, 1.891440f), // Control 2

        // Segment 18
        glm::vec3(16.598379f, -0.005880f, 1.314054f), // Anchor 18
        glm::vec3(14.616126f, -0.005281f, 1.206135f), // Control 1
        glm::vec3(12.810913f, -0.004334f, 1.100719f), // Control 2

        // Segment 19
        glm::vec3(-1.195549f, -0.002268f, 0.320951f) // Anchor 19

    };
    std::vector<std::string> skyboxFaces = {
        "../Models/cubeMap/posx.jpg",
        "../Models/cubeMap/negx.jpg",
        "../Models/cubeMap/posy.jpg",
        "../Models/cubeMap/negy.jpg",
        "../Models/cubeMap/posz.jpg",
        "../Models/cubeMap/negz.jpg"};
    DrawableObject *cube = createObject("cube", "skyboxShader");
    cube->getTransformation().setScale(glm::vec3(50));
    cube->getMaterial()->setFaces(skyboxFaces);
    cube->getMaterial()->loadCubeMap();

    Scene *scena = new Scene();
    DrawableObject *formula = createObject("formula", "phong");
    formula->getMaterial()->setObjectColor(glm::vec3(1.f, 0.0f, 0.0f));
    formula->getMaterial()->setShininess(32.0f);
    DrawableObject *raceTrack = createObject("raceTrack", "phong");
    formula->createBezier(racePoints, 1.0f, true);
    Directional *light = new Directional(glm::vec3(0.0f, 10.0f, -2.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    // scena->addLight(light);a
    scena->addObject(cube);
    scena->addLight(light);
    scena->addObject(raceTrack);
    scena->addObject(formula);

    this->createScene(scena);
}
void SceneBuilder::createGame()
{
    std::vector<std::string> skyboxFaces = {
        "../Models/NASA/cube/stars_posx.jpg",
        "../Models/NASA/cube/stars_negx.jpg",
        "../Models/NASA/cube/stars_posy.jpg",
        "../Models/NASA/cube/stars_negy.jpg",
        "../Models/NASA/cube/stars_posz.jpg",
        "../Models/NASA/cube/stars_negz.jpg"};
    DrawableObject *cube = createObject("cube", "skyboxShader");
    cube->getMaterial()->setFaces(skyboxFaces);
    cube->getTransformation().setScale(glm::vec3(50));
    cube->getMaterial()->loadCubeMap();
    DrawableObject *asteroid = createObject("asteroid", "constant");
    // asteroid->getMaterial()->loadTexture("../Models/NASA/2k_sun.jpg");
    asteroid->getTransformation().setScale(glm::vec3(0.00001f));
    GameScene *scene = new GameScene(4.0f, this->camera, asteroid);

    Directional *light = new Directional(glm::vec3(0.0f, 10.0f, -2.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    //  scene->addLight(light);
    scene->addObject(cube);
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
void SceneBuilder::setActiveSceneIndex(int8_t index)
{
    if (index >= 0 && index < scenes.size())
    {
        activeSceneIndex = index;
    }
}