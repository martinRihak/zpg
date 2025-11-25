#include "GameScene.hpp"

GameScene::GameScene(float spawnInterval, Camera *camera, DrawableObject *enemyPrototype)
    : spawnInterval(spawnInterval), camera(camera), enemyPrototype(enemyPrototype)
{
    this->isGame = true;
}
GameScene::~GameScene()
{
}
void GameScene::render(float dt)
{
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (DrawableObject *o : this->objects)
    {
        glStencilFunc(GL_ALWAYS, o->getID(), 0xFF);
        o->draw(dt, this->lights);
    }
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    for (DrawableObject *o : this->objects)
    {
        if (o->getHit() == true)
        {
            std::cout << o->getID()<< ": Byl smazan" << std::endl;
            objects.erase(std::remove(objects.begin(), objects.end(), o), objects.end());
            delete o;
            continue;
        }
        o->draw(dt, this->lights);
    }

    updateSpawning(dt);
    glDisable(GL_STENCIL_TEST);
}
void GameScene::updateSpawning(float dt)
{

    if (!camera || !enemyPrototype)
        return;

    spawnTimer += dt;

    // Náhodný interval mezi 0.8s – 2.5s pro větší zajímavost
    float currentInterval = spawnInterval + (rand() % 1000 / 1000.0f) * 2.0f - 0.5f;

    if (spawnTimer >= currentInterval)
    {
        spawnTimer = 0.0f;
        DrawableObject *enemy = enemyPrototype->clone();

        float distance = 25.0f + static_cast<float>(rand() % 1000) / 1000.0f * 20.0f;

        float theta = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
        float phi = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;

        glm::vec3 offset(
            distance * sin(phi) * cos(theta),
            distance * sin(phi) * sin(theta),
            distance * cos(phi));

        glm::vec3 cameraPos = camera->getPosition();
        enemy->getTransformation().setPosition(cameraPos + offset);

        float approachSpeed = 0.5f + static_cast<float>(rand() % 1000) / 1000.0f * 1.6f;

        enemy->addAnimator(new ApproachCameraAnimator(camera, approachSpeed, enemy->getHit(),2.0f));
        this->addObject(enemy);
    }
}