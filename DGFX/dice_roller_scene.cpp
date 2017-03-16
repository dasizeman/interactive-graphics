#include "dice_roller_scene.hpp"
namespace dgfx {
     const std::string DiceRollerScene::VERTEX_LIGHTING_SHADER_NAME = "phong_sun_spot_vert";
     const std::string DiceRollerScene::FRAGMENT_LIGHTING_SHADER_NAME = "phong_sun_spot_frag";
     const std::string DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME = "phong_sun_spot_frag_tex";
     
     DiceRollerScene::DiceRollerScene() : Scene() {
        //addShader( VERTEX_LIGHTING_SHADER_NAME );
        addShader( FRAGMENT_LIGHTING_SHADER_NAME );
        addShader( FRAGMENT_TEXTURE_SHADER_NAME );

        // Create cameras
        m_cameras.push_back( std::shared_ptr<Camera> ( new Camera (
                    0,
                    vec4( 0, 1, 0, 1 ),
                    vec4( 0, 0, -1, 0 ),
                    vec4( 0, 1, 0, 0 ) ) ) );

        m_activeCamera = m_cameras[0];

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


     void DiceRollerScene::clickHandler(GLint button, GLint state, GLint x, GLint y){}
     
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


}
