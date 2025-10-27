#pragma once
#include "Scene.hpp"
#include "../Camera.hpp"
#include <map>
#include <memory>
#include <vector>
#include "../PointLight.hpp"
class SceneBuilder
{
private:
    std::map<std::string, Model *> models;
    std::map<std::string, ShaderProgram *> shaders;
    std::vector<Light*> lights;
    Camera *camera;
    std::vector<Scene*> scenes;
    int8_t activeSceneIndex = 0;
    void createScene(Scene* scene);
    DrawableObject* createObject(const std::string& modelName, const std::string& shaderName);

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
    // Accessors
    Scene* getScene(int8_t index) const;
    int8_t getSceneCount() const;
    int8_t getActiveSceneIndex() const;
    void setActiveSceneIndex(int8_t index);
};