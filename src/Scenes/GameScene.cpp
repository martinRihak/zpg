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
    updateSpawning(dt);
    colision();
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (DrawableObject *o : this->objects)
    {
        glStencilFunc(GL_ALWAYS, o->getID(), 0xFF);
        o->draw(dt, this->lights);
    }
    for (DrawableObject *e : enemy)
    {
        e->draw(dt, this->lights);
    }
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    for (DrawableObject *o : this->objects)
    {
        o->draw(dt, this->lights);
    }
    for (DrawableObject *e : enemy)
    {
        e->draw(dt, this->lights);
    }
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

        enemy->addAnimator(new ApproachCameraAnimator(camera, approachSpeed, 2.0f));
        this->enemy.push_back(enemy);
    }
}

void GameScene::colision()
{
    for (DrawableObject *bull : objects)
    {
        for (DrawableObject *enemy : this->enemy)
        {
            if (bull->collidesWith(enemy))
            {
                printf("Kolize: Strela Zasahla nepritele.");
                removeObjects(enemy);
                objects.erase(std::remove(objects.begin(), objects.end(), bull), objects.end());
                delete bull;
                score += 10;
                printf("Skore: %d\n", score);
                return;
            }
        }
    }
}
void GameScene::removeObjects(DrawableObject *obj)
{
    enemy.erase(std::remove(enemy.begin(), enemy.end(), obj), enemy.end());
    delete obj;
}