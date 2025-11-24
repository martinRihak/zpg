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

        float newAlpha = camera->getAlpha() + yoffset;
        float newFi = camera->getFi() + xoffset;

        camera->setAngels(newAlpha, newFi);

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
                active = static_cast<int8_t>(i);
                camera->notifyAll();
            }
        }
        prevKeyState[i] = pressed;
    }

    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS && selectedObject)
    {
        Scene *currentScene = builder->getScene(active);
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
        ypos = height - ypos;

        glm::vec3 screenNear(2.0f * xpos / width - 1.0f, 2.0f * ypos / height - 1.0f, -1.0f);

        glm::mat4 proj = camera->getProjectionMatrix();
        glm::mat4 view = camera->getCamera();
        glm::vec4 viewport(0, 0, width, height);

        glm::vec3 nearPoint = glm::unProject(screenNear, view, proj, viewport);
        glm::vec3 rayOrigin = camera->getPosition();
        glm::vec3 rayDir = glm::normalize(nearPoint - rayOrigin);

        if (std::abs(rayDir.y) > 1e-6f)
        {
            float t = -rayOrigin.y / rayDir.y;
            if (t > 0.0f)
            {
                glm::vec3 plantPos = rayOrigin + t * rayDir;
                DrawableObject *newObj = nullptr;
                newObj = selectedObject->clone();
                if (newObj)
                {
                    newObj->getTransformation().setPosition(plantPos);
                    Scene *currentScene = builder->getScene(active);
                    if (currentScene)
                    {
                        currentScene->addObject(newObj);
                        printf("Strom sazen na pozici: (%f, %f, %f)\n", plantPos.x, plantPos.y, plantPos.z);
                    }
                }
            }
        }
    }

    if (isDragging && selectedObject && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 currentMousePos(xpos, ypos);
        if (glm::distance2(currentMousePos, lastMousePos) > 0.1f)
        {
            {
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                ypos = height - ypos;
                ypos = height - ypos;

                glm::vec3 screenNear(2.0f * xpos / width - 1.0f, 2.0f * ypos / height - 1.0f, -1.0f);
                glm::vec3 screenFar(screenNear.x, screenNear.y, 1.0f);

                glm::mat4 proj = camera->getProjectionMatrix();
                glm::mat4 view = camera->getCamera();
                glm::vec4 viewport(0, 0, width, height);

                glm::vec3 rayOrigin = glm::unProject(screenNear, view, proj, viewport);
                glm::vec3 rayEnd = glm::unProject(screenFar, view, proj, viewport);
                glm::vec3 rayDir = glm::normalize(rayEnd - rayOrigin);

                float objY = selectedObject->getTransformation().getPosition().y;
                if (std::abs(rayDir.y) > 1e-6f)
                {
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

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        GLbyte color[4];
        GLfloat depth;
        GLuint stencilId;

        glReadPixels((GLint)xpos, newY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels((GLint)xpos, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels((GLint)xpos, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilId);

        printf("Clicked on pixel %f, %f, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
               xpos, ypos, color[0], color[1], color[2], color[3], depth, stencilId);

        selectedObject = scene->getObjectById(stencilId);
        if (selectedObject)
        {
            printf("Vybrán objekt s ID %u\n", stencilId);
        }
        else
        {
            selectedObject = nullptr;
        }

        glm::vec3 screenPos((GLfloat)xpos, (GLfloat)newY, depth);
        glm::mat4 viewMatrix = camera->getCamera();
        glm::mat4 projMatrix = camera->getProjectionMatrix();
        glm::vec4 viewport(0, 0, (GLfloat)width, (GLfloat)height);
        glm::vec3 worldPos = glm::unProject(screenPos, viewMatrix, projMatrix, viewport);
        printf("unProject [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);
    }
}