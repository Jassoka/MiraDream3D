//
// Created by jassoka on 5/30/26.
//

#include "model/Camera.h"
#include "defs.h"

#include <cstdio>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 up,
               const glm::vec3 right,
               const glm::vec3 lookAt,
               const glm::vec3 position,
               const double FOV,
               const double nearPlane,
               const double farPlane,
               const double aspectRatio):
    mFOV(FOV), mNearPlane(nearPlane), mFarPlane(farPlane), mAspectRatio(aspectRatio),
    mAnchorPoint(WORLD_ORIGIN)
{
    const glm::mat4 translatedRotationMatrix(
        glm::vec4(right, 0.0),
        glm::vec4(up, 0.0),
        glm::vec4(lookAt, 0.0),
        glm::vec4(0.0, 0.0, 0.0, 1.0)
        );
    mRotationMatrix = glm::transpose(translatedRotationMatrix);
    mTranslationMatrix = glm::translate(glm::mat4(1.0), -position);
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

glm::vec3 Camera::getPosition() {
    return(-glm::vec3(mTranslationMatrix[3]));
}



void Camera::rotateAroundAnchor(const float dPhi, const float dTheta)
{
    const float rho = glm::length(glm::vec3(mTranslationMatrix[3]) - mAnchorPoint);
    const float cP = std::cos(dPhi);
    const float sP = std::sin(dPhi);
    const float cT = std::cos(dTheta);
    const float sT = std::sin(dTheta);

    const glm::mat3 rotYaw(
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, cT,   sT),
        glm::vec3(0.0f, -sT,  cT)
    );

    const glm::mat3 rotPitch(
        glm::vec3(cP,   0.0f, -sP),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(sP,   0.0f, cP)
        );

    auto R = glm::mat3(mRotationMatrix); // Rotation matrix

    R = rotPitch * rotYaw * R;

    const glm::vec3 newTranslation = rho * glm::normalize( glm::vec3(R[0][2], R[1][2], R[2][2])) + mAnchorPoint;


    mTranslationMatrix[3] = glm::vec4(newTranslation, 1.0f);
    mRotationMatrix       = glm::mat4(R);

}

void Camera::zoom(float zoomFactor) {
    glm::vec3 translation = (glm::vec3(mTranslationMatrix[3]) - mAnchorPoint)* zoomFactor + mAnchorPoint;
    mTranslationMatrix[3] = glm::vec4(translation,1.0f);

}

void Camera::strafeCamera(float dx,float dy) {
    glm::vec3 right = glm::vec3(mRotationMatrix[0][0], mRotationMatrix[1][0], mRotationMatrix[2][0]);
    glm::vec3 up    = glm::vec3(mRotationMatrix[0][1], mRotationMatrix[1][1], mRotationMatrix[2][1]);

    glm::vec3 translation=dx*right+dy*up;
    mAnchorPoint += translation;
    mTranslationMatrix[3]+=glm::vec4(translation,0.0);

}
