#include "Camera.hpp"

Camera::Camera()
    : eye(0.0f, 0.0f, -0.5f), // V�choz� pozice kamery
      up(0.0f, 1.0f, 0.0f),
      alpha(85.0f),
      fi(-90.0f)
{
    update();
}
glm::mat4 Camera::getCamera() const
{
    return glm::lookAt(eye, eye + target, up);
}
glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

void Camera::update()
{
    target.x = sin(glm::radians(alpha)) * cos(glm::radians(fi));
    target.y = cos(glm::radians(alpha));
    target.z = sin(glm::radians(alpha)) * sin(glm::radians(fi));
    target = glm::normalize(target);
}

void Camera::setEye(const glm::vec3 &eye)
{
    this->eye = eye;
    std::cout << eye.x << " " << eye.y << " " << eye.z << std::endl;
    update();
    notifyAll();
}
void Camera::setAngels(float alpha, float fi)
{
    this->alpha = glm::clamp(alpha, -89.0f, 89.0f);
    this->fi = fi;
    std::cout << alpha << " | " << fi << std::endl;
    update();
    notifyAll();
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