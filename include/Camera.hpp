#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include <ext/matrix_transform.hpp>

class Camera {
    public:
        Camera();

        // The ultimate view matrix we will produce and return.
        glm::mat4 GetViewMatrix() const {
            return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
        }

        void MouseLook(int mouseX, int mouseY);
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);

    private:
        glm::vec3 mEye;
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;

        glm::vec2 mOldMousePosition;
};

#endif