#pragma once
#include "Scene.hpp"
#include "../Camera.hpp"
#include "../Controller.hpp"
class GameScene : public Scene
{
private:
    int score = 0;
    float spawnTimer = 0;
    float spawnInterval = 0;
    Camera* camera = nullptr;
    DrawableObject* enemyPrototype = nullptr;
    std::vector<DrawableObject*> enemy;
    void colision();
    void updateSpawning(float dt);
public:
    GameScene(float spawnInterval, Camera* camera, DrawableObject* enemyPrototype);
    ~GameScene();
    void render(float dt) override;
};