#include "Scene.hpp"
#include <glm/glm.hpp>
#include <algorithm>
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
    obj->setID(countObjects++);
    printf("%d \n", countObjects);
    this->objects.push_back(obj);
}
void Scene::addLight(Light *light)
{
    lights.push_back(light);
}
DrawableObject* Scene::getObjectById(uint8_t id) {
    for (DrawableObject* obj : objects) {
        if (obj->getID() == id) return obj;
    }
    return nullptr;
}
void Scene::render(float dt)
{
glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    for (DrawableObject *o : this->objects)
    {
        glStencilFunc(GL_ALWAYS, o->getID(), 0xFF);  // Nastav ID pro objekt
        o->draw(dt, this->lights);  // Kresli objekt (ale jen stencil)
    }

    // Pass 2: Normální render s stencil testem (pokud potřebuješ, ale pro picking stačí pass 1 + read)
    glStencilFunc(GL_ALWAYS, 0, 0xFF);  // Ignoruj stencil pro normální render
    for (DrawableObject *o : this->objects)
    {
        o->draw(dt, this->lights);
    }

    glDisable(GL_STENCIL_TEST);
}
const std::vector<Light *> &Scene::getLights() const
{
    return lights;
}
DrawableObject *Scene::pickObject(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, float &dist)
{
    DrawableObject *closest = nullptr;
    dist = std::numeric_limits<float>::max();
    for (DrawableObject *obj : objects)
    {
        float tempDist = std::numeric_limits<float>::max();
        if (obj->intersectsRay(rayOrigin, rayDir, tempDist))
        {
            if (tempDist < dist)
            {
                closest = obj;
                dist = tempDist;
            }
        }
    }
    return closest;
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