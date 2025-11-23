#pragma once
#include "Scene.hpp"
#include "../Camera.hpp"
#include <map>
#include <memory>
#include <vector>
#include "../Lights/PointLight.hpp"
#include "../Lights/Reflector.hpp"
#include "../Lights/Directional.hpp"

class SceneBuilder
{
private:
    std::map<std::string, Model *> models;
    std::map<std::string, ShaderProgram *> shaders;
    std::vector<Light *> lights;
    Camera *camera;
    std::vector<Scene *> scenes;
    int8_t activeSceneIndex = 0;

    void createScene(Scene *scene);

public:
    SceneBuilder(Camera *camera);
    ~SceneBuilder();

    void registerModel(const std::string &name, Model *model);
    void registerShader(const std::string &name, ShaderProgram *shader);
    void createTriangle();
    void create4Spheres();
    void createForest();
    void createSunSystem();
    void createTestScene();
    void createShrek();
    // Accessors

    Model *getModel(const std::string &name) { return models[name]; }
    ShaderProgram *getShader(const std::string &name) const;

    DrawableObject *createObject(const std::string &modelName, const std::string &shaderName);
    Scene *getScene(int8_t index) const;
    int8_t getSceneCount() const;
    int8_t getActiveSceneIndex() const;
    void setActiveSceneIndex(int8_t index);
};