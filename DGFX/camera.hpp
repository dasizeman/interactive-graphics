#ifndef __DGFX_CAMERA_H__
#define __DGFX_CAMERA_H__
#include "Angel.h"
namespace dgfx {
class Camera {

private:
    float m_eye, m_u, m_v, m_n;
    bool m_usePerspectiveProjection = false;
    void updateViewMatrix();

public:
    Camera();

    mat4 m_viewMatrix;
    mat4 m_projectionMatrix;

    void toggleProjectionMode();
    void usePerspectiveProjection( float fovy, float aspect, float znear, float zfar );
    void useOrthographicProjection( float left, float right, float bottom, float top, float near, float far );


    void moveAlongAt( float amount );
    void moveAlongU( float amount );
    void pitch( float amount );
    void roll( float amount );
    void yaw( float amount );
};

}

#endif
