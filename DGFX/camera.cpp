#include "camera.hpp"

namespace dgfx {
    Camera::Camera( uint16_t id,
                    vec4 eye,
                    vec4 at,
                    vec4 up)  :
                    m_id( id ),
                    m_eye( eye ),
                    m_n( -at ),
                    m_v( up ){
        toggleProjectionMode();
        m_u = cross(m_v,m_n);
        m_u.w = 0;
        updateViewMatrix();
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
        m_fov = fovy;
        m_aspect = aspect;
        m_near = znear;
        m_far = zfar;

    }
    void Camera::useOrthographicProjection( float left, float right, float bottom, float top, float near, float far ) {
        m_projectionMatrix = Ortho( left, right, bottom, top, near, far );
    }


    void Camera::updateViewMatrix() {
        m_viewMatrix = LookAt( m_eye, m_eye- m_n, m_v );
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
        m_u = cross(m_v,m_n) ;
        m_u.w = 0;
        updateViewMatrix();
    }
    void Camera::roll( float amount ) {
        vec4 old_v = m_v;
        m_v = cos(amount)*old_v - sin(amount)*m_u;
        m_u = sin(amount)*old_v + cos(amount)*m_u;
        m_n = cross(m_u, m_v);
        m_n.w = 0;
        updateViewMatrix();

    }
    void Camera::yaw( float amount ) {
        vec4 old_u = m_u;
        m_u = cos(amount)*old_u - sin(amount)*m_n;
        m_n = sin(amount)*old_u + cos(amount)*m_n;
        m_v = cross(m_n, m_u);
        m_v.w = 0;
        updateViewMatrix();
    }


    mat4 Camera::viewInverse() {
        return mat4 (
            vec4( m_u.x, m_v.x, m_n.x, m_eye.x ),
            vec4( m_u.y, m_v.y, m_n.y, m_eye.y ),
            vec4( m_u.z, m_v.z, m_n.z, m_eye.z ),
            vec4( 0, 0, 0, 1 ) );
    }
}
