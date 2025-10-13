#include "Scene.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
}
void Scene::addObject(DrawableObject *obj)
{
    this->objects.push_back(obj);
}

void Scene::render(float dt)
{
    for (DrawableObject *o : this->objects)
    {
        o->draw(dt);
    }
}

void Scene::randomForest(glm::vec3 center, int radius, const std::vector<std::pair<DrawableObject *, int>> &objectsToSpawn)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (const auto &pair : objectsToSpawn)
    {
        DrawableObject *prototype = pair.first;
        int count = pair.second;

        for (int i = 0; i < count; ++i)
        {
            DrawableObject *newObj = prototype->clone();

            float randX = static_cast<float>(std::rand() % (2 * radius + 1) - radius);
            float randZ = static_cast<float>(std::rand() % (2 * radius + 1) - radius);
            glm::vec3 pos = glm::vec3(center.x + randX, center.y, center.z + randZ);

            float randAngle = static_cast<float>(std::rand() % 360);

            float randScale = 0.4f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (0.8f - 0.4f)));
            newObj->getTransformation().setPosition(pos);
            newObj->getTransformation().setRotation(randAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            newObj->getTransformation().setScale(glm::vec3(randScale));

            this->addObject(newObj);
        }
    }
}