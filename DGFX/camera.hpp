#ifndef __DGFX_CAMERA_H__
#define __DGFX_CAMERA_H__
#include "Angel.h"
namespace dgfx {
class Camera {

private:
    vec4 m_u, m_v, m_n;
    bool m_usePerspectiveProjection = false;
    void updateViewMatrix();

public:
    Camera(uint16_t id, vec4 eye, vec4 at, vec4 up);
    vec4 m_eye;
    uint16_t m_id;
    float m_near, m_far, m_aspect, m_fov;

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

    mat4 viewInverse();
};

}

#endif
