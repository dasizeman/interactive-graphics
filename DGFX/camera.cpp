#include "camera.hpp"

namespace dgfx {
    Camera::Camera() {
        toggleProjectionMode();
        m_viewMatrix = LookAt( vec4( 0,0,0,0), vec4(0,0,-1,0), vec4(0,1,0,0));
    }

    void Camera::toggleProjectionMode() {
        // TODO maybe don't hardcode this
        m_usePerspectiveProjection = !m_usePerspectiveProjection;
        if ( m_usePerspectiveProjection )
            usePerspectiveProjection( 65, 1, 1, 100 );
        else
            useOrthographicProjection( -1, 1, -1, 1, 1, 3 );
    
    }
    void Camera::usePerspectiveProjection( float fovy, float aspect, float znear, float zfar ) {
        m_projectionMatrix = Perspective( fovy, aspect, znear, zfar );

    }
    void Camera::useOrthographicProjection( float left, float right, float bottom, float top, float near, float far ) {
        m_projectionMatrix = Ortho( left, right, bottom, top, near, far );
    }


    void Camera::moveAlongAt( float amount ) {

    }
    void Camera::moveAlongU( float amount ) {

    }
    void Camera::pitch( float amount ) {

    }
    void Camera::roll( float amount ) {

    }
    void Camera::yaw( float amount ) {

    }
}
