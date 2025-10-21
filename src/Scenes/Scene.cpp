#include "Scene.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
    for (DrawableObject *obj : objects)
    {
        delete obj;
    }
    for (Light *light : lights)
    {
        delete light;
    }
}
void Scene::addObject(DrawableObject *obj)
{
    this->objects.push_back(obj);
}
void Scene::addLight(Light *light)
{
    lights.push_back(light);
   /* for (DrawableObject *obj : objects)
    {
        light->attach(obj->getShaderProgram());
    }*/
}
void Scene::render(float dt)
{
    for (DrawableObject *o : this->objects)
    {
        if (lights.empty())
        {
            o->draw(dt);
        }
        else
        {
            o->draw(dt, this->lights);
            //std::cout << "Su tu" << std::endl;
        }
    }
}
const std::vector<Light *> &Scene::getLights() const
{
    return lights;
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