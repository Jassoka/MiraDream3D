//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_CAMERA_HPP
#define MIRADREAM3D_CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 up,
        glm::vec3 right,
        glm::vec3 lookAt,
        glm::vec3 position,
        double FOV,
        double nearPlane,
        double farPlane,
        double aspectRatio);

    const glm::mat4& computePerspectiveMatrix();
    const glm::mat4& computeViewMatrix();

    void setFov(const double fov)
    {
        mFOV = fov;
    }

    void setNearPlane(const double near_plane)
    {
        mNearPlane = near_plane;
    }

    void setFarPlane(const double far_plane)
    {
        mFarPlane = far_plane;
    }

    void setAspectRatio(const double aspect_ratio)
    {
        mAspectRatio = aspect_ratio;
    }

    glm::vec3 getPosition();
    /**
     * @brief
     * @param dPhi Horizontal translation
     * @param dTheta Vertical translation
     */
    void rotateAroundAnchor(const float dPhi, const float dTheta);
    void zoom(float zoomFactor);
    void strafeCamera(float dx,float dy);

private:
    glm::mat4 mRotationMatrix = glm::mat4();
    glm::mat4 mTranslationMatrix = glm::mat4();
    glm::mat4 mViewMatrix = glm::mat4();
    glm::mat4 mPerspectiveMatrix = glm::mat4();
    double mFOV;
    double mNearPlane;
    double mFarPlane;
    double mAspectRatio;
    glm::vec3 mAnchorPoint = glm::vec3(0.0);
};



#endif //MIRADREAM3D_CAMERA_HPP
