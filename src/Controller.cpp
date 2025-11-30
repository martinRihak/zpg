#include "Controller.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <limits>

Controller::Controller()
{
    for (int i = 0; i < 9; ++i)
        prevKeyState[i] = false;
    prevBState = false;  // Inicializace pro klávesu B
    bezierMode = false;  // Výchozí: Bézier režim deaktivován
}

Controller::~Controller() {}

int8_t Controller::getActiveScene() { return active; }

void Controller::processInput(GLFWwindow *window, int8_t sceneCount, Camera *camera, float dt, SceneBuilder *builder)
{
    float cameraSpeed = 5.0f * dt;
    float mouseSensitivity = 0.1f;

    if (!window)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->setEye(camera->getPosition() + cameraSpeed * camera->getTarget());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->setEye(camera->getPosition() - cameraSpeed * camera->getTarget());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->setEye(camera->getPosition() - glm::normalize(glm::cross(camera->getTarget(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->setEye(camera->getPosition() + glm::normalize(glm::cross(camera->getTarget(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera->switchFlash();

    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!isRightMouseButtonPressed)
        {
            lastX = xpos;
            lastY = ypos;
            isRightMouseButtonPressed = true;
        }

        float xoffset = (xpos - lastX) * mouseSensitivity;
        float yoffset = (ypos - lastY) * mouseSensitivity;

        if (currentScene && currentScene->isGameScene())
        {
            float newFi = camera->getFi() + xoffset;
            camera->setAngels(camera->getAlpha(), newFi);
        }
        else
        {
            float newAlpha = camera->getAlpha() + yoffset;
            float newFi = camera->getFi() + xoffset;
            camera->setAngels(newAlpha, newFi);
        }

        lastX = xpos;
        lastY = ypos;
    }
    else
    {
        isRightMouseButtonPressed = false;
    }

    for (int i = 0; i < 9; ++i)
    {
        int key = GLFW_KEY_1 + i;
        int state = glfwGetKey(window, key);
        bool pressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        if (pressed && !prevKeyState[i])
        {
            printf("%d\n", key);
            if (i < sceneCount)
            {
                this->currentScene = builder->getScene(i);
                this->active = i;
                camera->notifyAll();
            }
        }
        prevKeyState[i] = pressed;
    }

    // Nové: Detekce klávesy B pro toggle Bézier režimu
    int bState = glfwGetKey(window, GLFW_KEY_B);
    bool bPressed = (bState == GLFW_PRESS || bState == GLFW_REPEAT);
    if (bPressed && !prevBState)
    {
        bezierMode = !bezierMode;  // Toggle režimu
        printf("Bézier režim: %s\n", bezierMode ? "Aktivní (přidávání bodů levým klikem)" : "Deaktivní (standardní picking/unProject)");
    }
    prevBState = bPressed;

    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS && selectedObject)
    {
        if (currentScene)
        {
            auto &objs = currentScene->getObjects();
            objs.erase(std::remove(objs.begin(), objs.end(), selectedObject), objs.end());
            delete selectedObject;
            selectedObject = nullptr;
            printf("Objekt smazán!\n");
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && selectedObject != nullptr)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glm::vec2 currentMousePos(static_cast<float>(xpos), static_cast<float>(ypos));

        if (!isDragging)
        {
            lastMousePos = currentMousePos;
            isDragging = true;
        }
        else
        {
            glm::vec2 delta = currentMousePos - lastMousePos;
            glm::vec3 rayOrigin = camera->getPosition();
            glm::vec3 rayDir = glm::normalize(glm::unProject(glm::vec3(currentMousePos.x, height - currentMousePos.y, 0.0f), camera->getCamera(), camera->getProjectionMatrix(), glm::vec4(0, 0, width, height)) - rayOrigin);

            float objY = selectedObject->getTransformation().getPosition().y;

            if (std::abs(rayDir.y) > 1e-6)
            {
                float t = (objY - rayOrigin.y) / rayDir.y;
                if (t > 0.0f)
                {
                    glm::vec3 newPos = rayOrigin + t * rayDir;
                    selectedObject->getTransformation().setPosition(newPos);
                }
            }

            lastMousePos = currentMousePos;
        }
    }
    else
    {
        isDragging = false;
    }

    if (selectedObject != nullptr)
    {
        glm::vec3 deltaPos(0.0f);
        float speed = 2.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            deltaPos.x -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            deltaPos.x += speed * dt;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            deltaPos.z -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            deltaPos.z += speed * dt;

        if (glm::length(deltaPos) > 0.0f)
        {
            glm::vec3 newPos = selectedObject->getTransformation().getPosition() + deltaPos;
            selectedObject->getTransformation().setPosition(newPos);
        }
    }
}

void Controller::handleMouseClick(GLFWwindow *window, int button, int action, int mods, Camera *camera, Scene *scene, SceneBuilder *builder)
{
    float moveSpeed = 2.0f;
    if (action != GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            isDragging = false;
        }
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int newY = height - (int)ypos;

    if (button == GLFW_MOUSE_BUTTON_LEFT && scene->isGameScene())
    {
        glm::vec3 rayOrigin = camera->getPosition();

        float x = (2.0f * xpos) / width - 1.0f;
        float y = 1.0f - (2.0f * ypos) / height;
        glm::vec4 rayClip(x, y, -1.0f, 1.0f);
        glm::vec4 rayEye = glm::inverse(camera->getProjectionMatrix()) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(camera->getCamera()) * rayEye));
        DrawableObject *shot = builder->createObject("sphere", "phong");

        shot->getTransformation().setScale(glm::vec3(0.05f));
        glm::vec3 startPos = rayOrigin + rayWorld * 0.5f; 

        shot->addAnimator(new ShootAnimator(startPos,rayWorld, 10.0f,camera));
        scene->addObject(shot);
    }

    // Spočítej world pozici pomocí unProject (společné pro oba režimy)
    GLbyte color[4];
    GLfloat depth;
    GLuint stencilId;

    glReadPixels((GLint)xpos, newY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels((GLint)xpos, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels((GLint)xpos, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilId);

    glm::vec3 screenPos((GLfloat)xpos, (GLfloat)newY, depth);
    glm::mat4 viewMatrix = camera->getCamera();
    glm::mat4 projMatrix = camera->getProjectionMatrix();
    glm::vec4 viewport(0, 0, (GLfloat)width, (GLfloat)height);
    glm::vec3 worldPos = glm::unProject(screenPos, viewMatrix, projMatrix, viewport);
    printf("unProject [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);

    if (button == GLFW_MOUSE_BUTTON_LEFT && !scene->isGameScene()) {
        if (bezierMode) {
            if (selectedObject->hasBezier()) {
                selectedObject->addControlPoint(worldPos); 
                printf("Přidán Bézier bod: [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);
            } else {
                printf("Vyber objekt pro přidávání bodů!\n");
            }
        } else {
            printf("Clicked on pixel %f, %f, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
                   xpos, ypos, color[0], color[1], color[2], color[3], depth, stencilId);

            selectedObject = scene->getObjectById(stencilId);
            if (selectedObject)
            {
                printf("Vybrán objekt s ID %u\n", stencilId);
                if (currentScene->isGameScene())
                {

                    auto &objs = currentScene->getObjects();
                    objs.erase(std::remove(objs.begin(), objs.end(), selectedObject), objs.end());
                    delete selectedObject;
                    selectedObject = nullptr;
                    printf("Objekt smazán!\n");
                }
            }
            else
            {
                selectedObject = nullptr;
            }
        }
    }
}