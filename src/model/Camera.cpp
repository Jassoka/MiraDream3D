//
// Created by jassoka on 5/30/26.
//

#include "model/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 up,
    const glm::vec3 right,
    const glm::vec3 lookAt,
    const glm::vec3 position,
    const double FOV,
    const double nearPlane,
    const double farPlane,
    const double aspectRatio):
    mFOV(FOV), mNearPlane(nearPlane), mFarPlane(farPlane), mAspectRatio(aspectRatio)
{
    const glm::mat4 translatedRotationMatrix(
        glm::vec4(right, 0.0),
        glm::vec4(up, 0.0),
        glm::vec4(lookAt, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 1.0)
        );
    mRotationMatrix = glm::transpose(translatedRotationMatrix);
    mTranslationMatrix = glm::translate(glm::mat4(1.0), position);
}

/**
 * @return Returns View Matrix
 */
const glm::mat4& Camera::computeViewMatrix()
{
    mViewMatrix = mRotationMatrix * mTranslationMatrix;
    return mViewMatrix;
}

/**
 * @return Returns Perspective Matrix (projection)
 */
const glm::mat4& Camera::computePerspectiveMatrix()
{
    const double f = mFarPlane;
    const double n = mNearPlane;
    const double s = 1.0f / std::tan(mFOV / 2.0f);

    mPerspectiveMatrix = glm::mat4(
        s / mAspectRatio, 0.0f, 0.0f,        0.0f,
        0.0f,              s,    0.0f,        0.0f,
        0.0f,              0.0f, f / (f - n), 1.0f,
        0.0f,              0.0f, -(f * n) / (f - n), 0.0f
    );
    return mPerspectiveMatrix;
}
