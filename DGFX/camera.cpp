#include "camera.hpp"

namespace dgfx {
    Camera::Camera() {
        toggleProjectionMode();
        m_eye = vec4( 0, 0, 0, 0);
        m_v = vec4( 0, 1, 0, 0);
        m_n = vec4( 0, 0, 1, 0 );
        m_u = normalize( cross(m_v,m_n) );
        updateViewMatrix();
        std::cout << "Camera constructor" << std::endl;
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


    void Camera::updateViewMatrix() {
        m_viewMatrix = LookAt( m_eye, m_eye - m_n, m_v );
    }


    void Camera::moveAlongAt( float amount ) {
        // This is moving along -n
        m_eye -= amount * normalize(m_n);
        updateViewMatrix();
    }
    void Camera::moveAlongU( float amount ) {
        m_eye += amount * normalize(m_u);
        updateViewMatrix();

    }
    void Camera::pitch( float amount ) {
        vec4 old_v = m_v;
        m_v = cos(amount)*old_v - sin(amount)*m_n;
        m_n = sin(amount)*old_v + cos(amount)*m_n;
        updateViewMatrix();
    }
    void Camera::roll( float amount ) {
        vec4 old_v = m_v;
        m_v = cos(amount)*old_v - sin(amount)*m_u;
        m_u = sin(amount)*old_v + cos(amount)*m_u;
        updateViewMatrix();

    }
    void Camera::yaw( float amount ) {
        vec4 old_u = m_u;
        m_u = cos(amount)*old_u - sin(amount)*m_n;
        m_n = sin(amount)*old_u + cos(amount)*m_n;
        updateViewMatrix();
    }
}
