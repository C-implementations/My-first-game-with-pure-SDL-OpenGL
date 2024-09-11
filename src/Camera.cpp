#include "Camera.hpp"
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"

Camera::Camera() {
    // Assume, we are placed at the origin
    mEye = glm::vec3(0.0f, 0.0f, 0.0f);

    // Assume we are looking out into the world
    // NOTE: This is along '-z', because outherwise, we'd be looking behind us
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    // Assume we start on a perfect plane
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::MouseLook(int mouseX, int mouseY)
{
    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);

    static int firstLook = true;
    if (firstLook)
    {
        mOldMousePosition = currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = mOldMousePosition - currentMouse; // gives us a vector

    mViewDirection = glm::rotate(mViewDirection, glm::radians((float) mouseDelta.x), mUpVector);

    mOldMousePosition = currentMouse;
}

void Camera::MoveForward(float speed) {
    mEye -= mViewDirection * speed;
}

void Camera::MoveBackward(float speed) {
    mEye += (mViewDirection * speed);
}

void Camera::MoveLeft(float speed)
{
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    mEye -= (rightVector * speed);
}

void Camera::MoveRight(float speed)
{
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    mEye += (rightVector * speed);
}
