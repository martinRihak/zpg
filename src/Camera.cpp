#include "Camera.hpp"
#include "Lights/Reflector.hpp"

Camera::Camera()
    : eye(0.0f, 0.0f, -0.5f),
      up(0.0f, 1.0f, 0.0f),
      alpha(0.0f),
      fi(-90.0f)
{
    update();
}
void Camera::createFlashLight()
{
    this->flashLight = new Reflector(
        this->eye,
        glm::vec3(0.4, 0.4, 0.4),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(1.f,0.09f,0.032f),
        this->target,
        17.5f, 21.5f);
}
void Camera::switchFlash(){this->flashLight->switchLight();}
Reflector *Camera::getFlashLight() const { return this->flashLight; }
glm::mat4 Camera::getCamera() const
{
    return glm::lookAt(eye, eye + target, up);
}
glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(45.0f), this->aspectRatio, 0.1f, 100.0f);
}
void Camera::update()
{
    target.x = cos(glm::radians(fi)) * cos(glm::radians(alpha));
    target.y = sin(glm::radians(alpha));
    target.z = sin(glm::radians(fi)) * cos(glm::radians(alpha));
    target = glm::normalize(target);
}
void Camera::setAspectRatio(float ar)
{
    this->aspectRatio = ar;
    notifyAll();
}
void Camera::setEye(const glm::vec3 &eye)
{
    this->eye = eye;
    flashLight->FlashMovement(this);
    update();
    notifyAll();
}
void Camera::setAngels(float alpha, float fi)
{
    this->alpha = glm::clamp(alpha, -89.0f, 89.0f);
    this->fi = fi;
    flashLight->FlashMovement(this);
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
float Camera::getAspectRatio() const { return this->aspectRatio; }