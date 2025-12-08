#include "GameScene.hpp"

GameScene::GameScene(float spawnInterval, Camera *camera, DrawableObject *enemyPrototype,ShaderProgram* scenesShader)
    : spawnInterval(spawnInterval), camera(camera), enemyPrototype(enemyPrototype), scenesShader(scenesShader)
{
    this->isGame = true;
    createScenes();

}
GameScene::~GameScene()
{
}
void GameScene::createScenes()
{
    if(scenesShader == nullptr){
        printf("ShaderProgram neni prirazen\n");
        return;
    }
    this->scenesShader = scenesShader;
    gameOver = new Scene();
    Model* gameOverModel = new Model("../Models/Game/GameOver.obj");
    DrawableObject* gameOverObj = new DrawableObject(gameOverModel, scenesShader);
    gameOver->addObject(gameOverObj);

}
void GameScene::render(float dt)
{
    if (score < 0)
    {
        gameOver->render(dt);
        return;
    }
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
        glStencilFunc(GL_ALWAYS, e->getID(), 0xFF);
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

    float currentInterval = spawnInterval + (rand() % 1000 / 1000.0f) * 2.0f - 0.5f;

    if (spawnTimer >= currentInterval)
    {
        spawnTimer = 0.0f;
        DrawableObject *enemy = enemyPrototype->clone();

        glm::vec3 spawnPos;
        bool positionInView = false;
        int maxTries = 50; // Omezení počtu pokusů, aby se zabránilo nekonečné smyčce

        for (int i = 0; i < maxTries; ++i) {
            float distance = 15.0f + static_cast<float>(rand() % 1000) / 1000.0f * 10.0f;
            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;

            glm::vec3 cameraPos = camera->getPosition();
            // Generujeme pozici v kruhu kolem kamery na stejné výškové úrovni
            glm::vec3 offset(distance * cos(angle), 0.0f, distance * sin(angle));
            spawnPos = cameraPos + offset;

            // Test viditelnosti
            glm::vec4 clipSpacePos = camera->getProjectionMatrix() * camera->getCamera() * glm::vec4(spawnPos, 1.0);
            glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;

            if (ndcSpacePos.z > 0 && ndcSpacePos.z < 1.0 && // Je mezi near a far rovinou
                abs(ndcSpacePos.x) < 1.0 && abs(ndcSpacePos.y) < 1.0) { // Je vlevo/vpravo/nahoře/dole
                positionInView = true;
                break;
            }
        }

        if (!positionInView) { // Pokud se nepodařilo najít pozici, přeskočíme spawn
             delete enemy;
             return;
        }
        enemy->getTransformation().setPosition(spawnPos);
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
            if (enemy->isDestroyed())
            {
                removeObjects(enemy);
                score -= 10;
                printf("Skore: %d\n", score);
                continue;
            }
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