#include <cassert>

#include "dice_roller_scene.hpp"
#include "object.hpp"
#include "daveutils.hpp"
namespace dgfx {
     const std::string DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME = "phong_sun_spot_frag_tex";
     
     DiceRollerScene::DiceRollerScene(std::string windowName, int width, int height) : 
         Scene(windowName, width, height) {
        addShader( FRAGMENT_TEXTURE_SHADER_NAME );

        // Create cameras
        m_cameras.push_back( std::shared_ptr<Camera> ( new Camera (
                    0,
                    vec4( 0, 1, 0, 1 ),
                    vec4( 0, 0, -1, 0 ),
                    vec4( 0, 1, 0, 0 ) ) ) );

        m_activeCamera = m_cameras[0];

        // Make sure the aspect ratio is correct
        updateCameraAspectRatios( m_screenWidth, m_screenHeight );

        // Create the lights
        m_lights.push_back( Light(DIRECTIONAL, 
                    vec4(-1.0,0.0,0.0,0.0), 
                    vec4( 0.2, 0.2, 0.2, 1.0 ),
                    vec4( 1.0, 1.0, 1.0, 1.0 ),
                    vec4( 1.0, 1.0, 1.0, 1.0 )));


        m_lights.push_back( Light(SPOTLIGHT, 
                    m_activeCamera->m_eye, 
                    vec4( 0.2, 0.2, 0.2, 1.0 ),
                    vec4( 1.0, 1.0, 1.0, 1.0 ),
                    vec4( 1.0, 1.0, 1.0, 1.0 )));
     }

     void DiceRollerScene::updateCameraAspectRatios(int width, int height) {
        for (auto const camera : m_cameras) {
            camera->changeProjectionAspectRatio(width, height);
        }
     }

     void DiceRollerScene::keyboardHandler(unsigned char key, int x, int y) {
         Scene::keyboardHandler( key, x, y );
         const float ROTATION_SPEED = 0.1;

         if ( key == ' ' ) {
             m_lights[1].m_toggle = !m_lights[1].m_toggle;
         }

         switch ( key ) {
             case 'a':
                m_lights[0].m_toggle = !m_lights[0].m_toggle;
                break;
             case 'r':
                 globalAnimationToggle();
                 break;

             case 'p':
                m_activeCamera->toggleProjectionMode();
             break;

             case 'X':
                m_activeCamera->pitch( ROTATION_SPEED );
             break;
             case 'x':
                m_activeCamera->pitch( -ROTATION_SPEED );
            break;

             case 'Z':
                m_activeCamera->roll( ROTATION_SPEED );
             break;
             case 'z':
                m_activeCamera->roll( -ROTATION_SPEED );
             break;

             case 'C':
                m_activeCamera->yaw( ROTATION_SPEED );
             break;

             case 'c':
                m_activeCamera->yaw( -ROTATION_SPEED );
             break;

         }

     }


     void DiceRollerScene::clickHandler(GLint button, GLint state, GLint x, GLint y){
         if (! (m_activeCamera->m_id == 0 &&
                button == GLUT_LEFT_BUTTON &&
                state == GLUT_DOWN ))
             return;

         pickTriangle( x, y );
     }
     
     void DiceRollerScene::specialKeyHandler(int key, int x, int y) {
         Scene::specialKeyHandler( key, x, y );
         const float MOVE_SPEED = 0.25;

         if (m_activeCamera->m_id != 0 )
             return;

         switch (key) {
            case GLUT_KEY_UP:
                m_activeCamera->moveAlongAt( MOVE_SPEED );
            break;

            case GLUT_KEY_DOWN:
                m_activeCamera->moveAlongAt( -MOVE_SPEED );
            break;

            case GLUT_KEY_LEFT:
                m_activeCamera->moveAlongU( -MOVE_SPEED );
            break;

            case GLUT_KEY_RIGHT:
                m_activeCamera->moveAlongU( MOVE_SPEED );
            break;
         }

     }

     void DiceRollerScene::displayCallback(){
        for ( auto const &shader : m_shaderMap ) {
            GLuint mainShader = shader.second;

            // Set the view and projection uniforms
            glUseProgram( mainShader );
            GLuint mainProjMatrix = glGetUniformLocation( mainShader, "proj_matrix" );
            glUniformMatrix4fv(mainProjMatrix,1, GL_TRUE, m_activeCamera->m_projectionMatrix);
            GLuint mainViewMatrix = glGetUniformLocation( mainShader, "view_matrix" );
            glUniformMatrix4fv(mainViewMatrix,1, GL_TRUE, m_activeCamera->m_viewMatrix);

            // Set the lighting uniforms for the directional light
            GLuint shaderLoc = glGetUniformLocation( mainShader, "DirectionalLightPosition" );
            glUniform4fv( shaderLoc, 1, m_lights[0].m_position );
            shaderLoc = glGetUniformLocation( mainShader, "DirectionalLightAmbient" );
            glUniform4fv( shaderLoc, 1, m_lights[0].m_ambient );
            shaderLoc = glGetUniformLocation( mainShader, "DirectionalLightDiffuse" );
            glUniform4fv( shaderLoc, 1, m_lights[0].m_diffuse );
            shaderLoc = glGetUniformLocation( mainShader, "DirectionalLightSpecular" );
            glUniform4fv( shaderLoc, 1, m_lights[0].m_specular );
            shaderLoc = glGetUniformLocation( mainShader, "DirectionalLightToggle" );
            glUniform1i( shaderLoc, m_lights[0].m_toggle);

            // Set the lighting uniforms for the flash light
            shaderLoc = glGetUniformLocation( mainShader, "FlashlightPosition" );
            glUniform4fv( shaderLoc, 1, m_lights[1].m_position );
            shaderLoc = glGetUniformLocation( mainShader, "FlashlightAmbient" );
            glUniform4fv( shaderLoc, 1, m_lights[1].m_ambient );
            shaderLoc = glGetUniformLocation( mainShader, "FlashlightDiffuse" );
            glUniform4fv( shaderLoc, 1, m_lights[1].m_diffuse );
            shaderLoc = glGetUniformLocation( mainShader, "FlashlightSpecular" );
            glUniform4fv( shaderLoc, 1, m_lights[1].m_specular );
            shaderLoc = glGetUniformLocation( mainShader, "FlashlightToggle" );
            glUniform1i( shaderLoc, m_lights[1].m_toggle );
        }

        Scene::displayCallback();


     }

     void DiceRollerScene::timerCallback( int value ) {

         m_lights[0].m_position = RotateZ(0.25) * m_lights[0].m_position;
         m_lights[1].m_position = m_activeCamera->m_eye;
         Scene::timerCallback( value );

     }


     void DiceRollerScene::reshapeCallback( int width, int height ) {
        updateCameraAspectRatios( width, height );
     }

     void DiceRollerScene::pickTriangle( uint16_t x, uint16_t y ) {
         float xFloat = static_cast<float>(x);
         float yFloat = static_cast<float>(y);

         // First we calculate where the clicked point is on the front of the
         // canonical view volume
         vec4 canonicalPt ( 2 * (xFloat / m_screenWidth) - 1,
                            1 - 2 * (yFloat / m_screenHeight),
                            -1,
                            1 );

         // Now we undo the perspective projection
         float t = m_activeCamera->m_near * tan( ((M_PI / 180)  * m_activeCamera->m_fov) / 2) ;
         float r = t * m_activeCamera->m_aspect;

         vec4 noPerspective ( r * canonicalPt.x,
                              t * canonicalPt.y,
                              -m_activeCamera->m_near,
                              1 );

         // Now we need to get from camera coordinates to world coordinates by
         // creating and multiplying by the inverse of camera coord system
         mat4 viewInverse = m_activeCamera->viewInverse();

         vec4 worldPt = viewInverse * noPerspective;

         // Now we can get the click ray in world coordinates
         vec3 worldRay = daveutils::FourDto3d( worldPt - m_activeCamera->m_eye );

         // We now go through every triangle in the scene to see if our ray
         // intersects it
         for ( std::unique_ptr<Entity> &entity : m_entities ) {
             // This is a relic of the old codebase; we won't have any entities
             // in this project that aren't of class Object, but I'm too lazy to
             // refactor
            Object *object = dynamic_cast<Object*>( entity.get() );

            // But just in case...
            assert( object != nullptr );

            // Make sure we only call the event for the first face we hit
            bool hit = false;

             for ( uint16_t triangleIdx = 0; triangleIdx < object->m_vertices.size(); triangleIdx += 3 ) {
                 vec3 E = daveutils::FourDto3d( object->m_vertices[ triangleIdx ] );
                 vec3 F = daveutils::FourDto3d( object->m_vertices[ triangleIdx + 1 ] );
                 vec3 G = daveutils::FourDto3d( object->m_vertices[ triangleIdx + 2 ] );

                 float t = rayIntersectsPlane( worldRay, E, F, G );
                 if ( t > 0) {
                    vec3 intersectPt = daveutils::FourDto3d( m_activeCamera->m_eye ) + t * worldRay;
                    if ( insideTest( E, F, G, intersectPt ) ) {
                        object->wasPicked( triangleIdx );
                        hit = true;
                        break;
                    }
                 }

             }

         }

     }


     float DiceRollerScene::rayIntersectsPlane( vec3 ray, vec3 e, vec3 f, vec3 g ) {
         vec3 N = cross( (f - e), (g - e) );
         vec3 O = daveutils::FourDto3d( m_activeCamera->m_eye );
         vec3 D = ray;
         float d = dot( -e, N );

         return - ( dot(N, O) + d ) / dot(N,D);

     }


     bool DiceRollerScene::insideTest( vec3 e, vec3 f, vec3 g, vec3 point ) {
         vec3 N = cross( (f - e), (g - e) );
         float dot1 = dot( cross( f - e, point - e ), N );
         float dot2 = dot( cross( g - f, point - f ), N );
         float dot3 = dot( cross( e - g, point - g ), N );

         //std::cout << dot1 << "," << dot2 << "," << dot3 << std::endl;

         return dot1 >= 0 && dot2 >= 0 && dot3 >= 0;

     }



}
