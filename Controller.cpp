#include "Controller.hpp"

Controller::Controller()
{
    for (int i = 0; i < 9; ++i)
        prevKeyState[i] = false;
}

Controller::~Controller() {}

void Controller::processInput(GLFWwindow* window, std::vector<Scene*>& scenes, int8_t& active, int8_t sceneCount, Camera* camera) {
    float cameraSpeed = 0.05f;
    float mouseSensitivity = 0.1f; 
    if (!window)
        return;


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->setEye(camera->getPosition() + cameraSpeed * camera->getTarget());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->setEye(camera->getPosition() - cameraSpeed * camera->getTarget());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->setEye(camera->getPosition() + glm::normalize(glm::cross(camera->getTarget(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->setEye(camera->getPosition() - glm::normalize(glm::cross(camera->getTarget(), camera->getUp())) * cameraSpeed);


    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!isRightMouseButtonPressed) {
            lastX = xpos;
            lastY = ypos;
            isRightMouseButtonPressed = true;
        }

        float xoffset = static_cast<float>(xpos - lastX) * mouseSensitivity;
        float yoffset = static_cast<float>(lastY - ypos) * mouseSensitivity; 

     
        float newAlpha = camera->getAlpha() + yoffset; 
        float newFi = camera->getFi() - xoffset;       

        camera->setAngels(newAlpha, newFi);

        // Update last position
        lastX = xpos;
        lastY = ypos;
    } else {
        isRightMouseButtonPressed = false;
    }

    // Handle scene switching (1-9 keys)
    for (int i = 0; i < 9; ++i) {
        int key = GLFW_KEY_1 + i;
        int state = glfwGetKey(window, key);
        bool pressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        if (pressed && !prevKeyState[i]) {
            printf("%d\n", key);
            if (i < sceneCount) {
                active = static_cast<int8_t>(i);
            }
        }
        prevKeyState[i] = pressed;
    }
}