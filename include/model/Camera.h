//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_CAMERA_HPP
#define MIRADREAM3D_CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/**
 * @brief Class for creating cameras
 */
class Camera {
public:
    /**
     * @brief Default constructor for Camera
     * @param up Camera up direction
     * @param right Camera right direction
     * @param lookAt Direction the camera looks at
     * @param position Initial camera position
     * @param FOV @copybrief Camera::mFOV
     * @param nearPlane @copybrief Camera::mNearPlane
     * @param farPlane @copybrief Camera::mFarPlane
     * @param aspectRatio @copybrief Camera::mAspectRatio
     */
    Camera(glm::vec3 up,
        glm::vec3 right,
        glm::vec3 lookAt,
        glm::vec3 position,
        double FOV,
        double nearPlane,
        double farPlane,
        double aspectRatio);

    /**
     * @brief Computes and store perspective matrix in \ref mPerspectiveMatrix
     * @return Constant reference to matrix
     */
    const glm::mat4& computePerspectiveMatrix();
    /**
     * @brief Computes and store view matrix in \ref mViewMatrix
     * @return Constant reference to matrix
     */
    const glm::mat4& computeViewMatrix();

    /** @setter{\ref mFOV} */
    void setFov(const double fov) { mFOV = fov; }

    /** @setter{\ref mNearPlane} */
    void setNearPlane(const double near_plane) { mNearPlane = near_plane; }

    /** @setter{\ref mFarPlane} */
    void setFarPlane(const double far_plane) { mFarPlane = far_plane; }

    /** @setter{\ref mAspectRatio} */
    void setAspectRatio(const double aspect_ratio) { mAspectRatio = aspect_ratio; }

    glm::vec3 getPosition() const;
    glm::vec3 getLookAt() const ;
    glm::vec3 getAnchorPoint() const {return mAnchorPoint;}
    float getAnchorPointDistance() const { return glm::length(mAnchorPoint - getPosition());}

    /** @getter{\ref mFarPlane} */
    double getFarPlane() const;
    /** @getter{\ref mNearPlane} */
    double getNearPlane() const;

    /**
     * @brief Rotates the camera around \ref mAnchorPoint
     * @param dPhi Horizontal rotation
     * @param dTheta Vertical rotation
     */
    void rotateAroundAnchor(float dPhi, float dTheta);
    /**
     * @brief Zooms towards \ref mAnchorPoint
     */
    void zoom(float zoomFactor);


    /**
     * @brief Translates camera along right and up axis
     * @note Moves the anchor point along
     * @param dx Horizontal translation
     * @param dy Vertical translation
     */
    void strafeCamera(float dx,float dy);

private:
    /** @brief 4x4 Rotation matrix containing up, right and lookAt vectors */
    glm::mat4 mRotationMatrix;
    /** @brief 4x4 Rotation matrix containing camera position coordinates */
    glm::mat4 mTranslationMatrix;
    /** @brief Computed view matrix */
    glm::mat4 mViewMatrix;
    /** @brief Computed perspective matrix */
    glm::mat4 mPerspectiveMatrix;

    /** @brief Field of view (in radians) */
    double mFOV;
    /** @brief Minimum distance for objects to be captured by the camera */
    double mNearPlane;
    /** @brief Maximum distance for objects to be captured by the camera */
    double mFarPlane;
    /** @brief Camera aspect ratio (width/height) */
    double mAspectRatio;
    /** @brief Camera anchor point for rotation */
    glm::vec3 mAnchorPoint;
};



#endif //MIRADREAM3D_CAMERA_HPP
