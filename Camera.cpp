#include "Camera.hpp"

Camera::Camera()
    : eye(0.0f, 0.0f, -0.5f), // V�choz� pozice kamery
      up(0.0f, 1.0f, 0.0f),
      alpha(0.0f),
      fi(90.0f)
{
    update();
}
glm::mat4 Camera::getCamera() const
{
    return glm::lookAt(eye, eye + target, up);
}
glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(60.0f, aspectRatio, 0.1f, 100.0f);
}

void Camera::update()
{
    target.x = cos(glm::radians(fi)) * cos(glm::radians(alpha));
    target.y = sin(glm::radians(alpha));
    target.z = sin(glm::radians(fi)) * cos(glm::radians(alpha));
    target = glm::normalize(target);
}

void Camera::setEye(const glm::vec3 &eye)
{
    this->eye = eye;
    std::cout << eye.x << " " << eye.y << " " << eye.z << std::endl;
    update();
    notifyObservers();
}
void Camera::setAngels(float aplha, float fi)
{
    this->alpha = glm::clamp(alpha, -89.0f, 89.0f);
    this->fi = fi;
    std::cout << aplha << " | " << fi << std::endl;
    update();
    notifyObservers();
}
void Camera::addObserver(ShaderProgram *observer)
{
    this->observers.push_back(observer);
}
void Camera::notifyObservers()
{
    for (auto o : this->observers)
    {
        o->update();
    }
}
glm::vec3 Camera::getPosition() const
{
    return eye;
}
glm::vec3 Camera::getTarget() const
{
    return target;
}

glm::vec3 Camera::getUp() const
{
    return up;
}
float Camera::getAlpha() const
{
    return alpha;
}
float Camera::getFi() const
{
    return fi;
}