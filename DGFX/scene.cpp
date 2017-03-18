#include <sstream>
#include <cassert>

#include "scene.hpp"
#include "daveutils.hpp"
#include "geometry.hpp"

namespace dgfx {

    const std::string Scene::DEFAULT_WINDOW_NAME = "CS432";
    const std::string Scene::SHADER_PATH = "shaders/";
    const int Scene::DEFAULT_WINDOW_WIDTH = 500, Scene::DEFAULT_WINDOW_HEIGHT = 500;
    const std::string Scene::FLAT_3D_SHADER_NAME = "3d_model";
    const std::string Scene::WIREFRAME_SHADER_NAME = "3d_wireframe";

    std::shared_ptr<Scene> Scene::m_instance = nullptr;
    

    Scene::Scene() : 
        Scene::Scene( DEFAULT_WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT){}
    
    Scene::Scene(std::string windowName, int width, int height) : 
        m_screenWidth( width ), m_screenHeight( height ){
        init( windowName, width, height );
    }

    void Scene::init(std::string windowName, int width, int height) {
#ifdef __APPLE__
        glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
        glutInitWindowSize( width, height );

        glutCreateWindow( windowName.c_str() );
        glewInitAndVersion();


        // Assign event callbacks
        glutDisplayFunc( &Scene::display_callback_wrapper );
        glutMouseFunc( &Scene::click_callback_wrapper );
        glutKeyboardFunc( &Scene::keyboard_callback_wrapper );
        glutSpecialFunc( &Scene::special_key_wrapper );
        glutReshapeFunc( &Scene::reshape_callback_wrapper );
        glutWMCloseFunc( &Scene::close_handler );

        // 100 fps!
        glutTimerFunc( 1000, timer_callback_wrapper, 0);

        // Load the global 3D shaders
        // TODO handle this differently; they aren't always needed
        addShader( FLAT_3D_SHADER_NAME );
        addShader( WIREFRAME_SHADER_NAME );


        glEnable (GL_DEPTH_TEST);
        glClearColor( 1.0, 1.0, 1.0, 1.0 );

    }


    void Scene::addEntity(std::unique_ptr<Entity> entity) {
        entity->setShader( m_shaderMap );
        entity->init( m_shaderMap );

        m_entities.push_back( std::move(entity) );

    }

    void Scene::addShader ( std::string shaderName ) {
        if ( m_shaderMap.find( shaderName ) != m_shaderMap.end() )
            return;

        std::stringstream ss;
        ss << SHADER_PATH << shaderName << ".vert";
        auto vertexShaderPath = ss.str();

        ss.str("");
        ss << SHADER_PATH << shaderName << ".frag";
        auto fragmentShaderPath = ss.str();

        GLuint shader = InitShader( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
        std::cout << "Initialized shader ID " << shader << std::endl;
        m_shaderMap[ shaderName ] = shader;
    }
    
    void Scene::start() {
        assert( m_instance != nullptr );
        glutMainLoop();
    }

    void Scene::globalAnimationToggle() {
         m_doAnimation = !m_doAnimation;
        for ( std::unique_ptr<Entity> &entity : m_entities )
            entity->m_doAnimation = m_doAnimation;
    }

    void Scene::keyboardHandler(unsigned char key, int x, int y) {
        switch( key ) {
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        }

        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->keyboardHandler( key, x, y);

    }


    void Scene::specialKeyHandler(int key, int x, int y) {
        for (std::unique_ptr<Entity> &entity : m_entities )
            entity->specialKeyHandler( key, x, y);
    }

    void Scene::clickHandler(GLint button, GLint state, GLint x, GLint y) {
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->clickHandler( button, state, x, y);
    }

    void Scene::displayCallback() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->draw( m_shaderMap );

        glutSwapBuffers();
        //glFlush();
    }

    void Scene::timerCallback( int value ) {
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->update( m_shaderMap );
    }

    void Scene::reshapeCallback( int width, int height ){}


    void Scene::glewInitAndVersion(void)
    {
       fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
       fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef __APPLE__
       GLenum err = glewInit();
       if (GLEW_OK != err)
       {
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
       }
       fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    }


    // Static 
    void Scene::display_callback_wrapper() {
        Scene::m_instance->displayCallback();
    }

     void Scene::timer_callback_wrapper( int value ) {
        Scene::m_instance->timerCallback( value );
        glutTimerFunc( 1000/FPS, timer_callback_wrapper, 0 );
        glutPostRedisplay();
     }

     void Scene::reshape_callback_wrapper( int width, int height ) {
        Scene::m_instance->reshapeCallback( width, height );
     }

     void Scene::keyboard_callback_wrapper(unsigned char key, int x, int y) {
        Scene::m_instance->keyboardHandler( key, x, y );
     }

     void Scene::special_key_wrapper( int key, int x, int y ) {
        Scene::m_instance->specialKeyHandler( key, x, y );
     }

     void Scene::click_callback_wrapper(GLint button, GLint state, GLint x, GLint y) {
         Scene::m_instance->clickHandler( button, state, x, y );
     }

        void Scene::close_handler() {
    }


     


    /*
     // ----- A4Scene -----
     
     A4Scene::A4Scene() : Scene() {
        // Create cameras
        m_cameras.push_back( std::shared_ptr<Camera> ( new Camera (
                    0,
                    vec4( 0, 2, 0, 0 ),
                    vec4( 0, 0, -1, 0 ),
                    vec4( 0, 1, 0, 0 ) ) ) );

        m_cameras.push_back( std::shared_ptr<Camera> ( new Camera (
                    1,
                    vec4( 0, 10, 0, 0),
                    vec4( 0, -1, 0, 0),
                    vec4( 0, 0, -1, 0) ) ) );

        m_activeCamera = m_cameras[0];
     }
     
     void A4Scene::keyboardHandler(unsigned char key, int x, int y) {
         Scene::keyboardHandler( key, x, y );
         const float ROTATION_SPEED = 0.1;

         if ( key == ' ' ) {
             if ( m_activeCamera->m_id == 0 )
                 m_activeCamera = m_cameras[1];
             else
                 m_activeCamera = m_cameras[0];
         }

         if (m_activeCamera->m_id != 0 )
             return;

         switch ( key ) {
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

     void A4Scene::specialKeyHandler(int key, int x, int y) {
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

     void A4Scene::clickHandler(GLint button, GLint state, GLint x, GLint y) {
        // TODO testing
        //for (std::unique_ptr<Entity> &entity : m_entities ) {
        //    RecursiveSphere *sphere = dynamic_cast<RecursiveSphere*>( entity.get() );
        //    if ( sphere != nullptr )
        //        sphere->blackenTriangle( m_miscIdx );
        //}
        //m_miscIdx++;
         if (! (m_activeCamera->m_id == 0 &&
                button == GLUT_LEFT_BUTTON &&
                state == GLUT_DOWN ))
             return;

         pickTriangle( x, y );

     }

     void A4Scene::displayCallback() {
        GLuint mainShader = m_shaderMap[ Scene::FLAT_3D_SHADER_NAME ];
        GLuint wireframeShader = m_shaderMap[ Scene::WIREFRAME_SHADER_NAME ];

        glUseProgram( mainShader );
        GLuint mainProjMatrix = glGetUniformLocation( mainShader, "proj_matrix" );
        glUniformMatrix4fv(mainProjMatrix,1, GL_TRUE, m_activeCamera->m_projectionMatrix);
        GLuint mainViewMatrix = glGetUniformLocation( mainShader, "view_matrix" );
        glUniformMatrix4fv(mainViewMatrix,1, GL_TRUE, m_activeCamera->m_viewMatrix);

        glUseProgram( wireframeShader );
        GLuint wireframeProjMatrix = glGetUniformLocation( wireframeShader, "proj_matrix" );
        glUniformMatrix4fv(wireframeProjMatrix,1, GL_TRUE, m_activeCamera->m_projectionMatrix);
        GLuint wireframeViewMatrix = glGetUniformLocation( mainShader, "view_matrix" );
        glUniformMatrix4fv(wireframeViewMatrix,1, GL_TRUE, m_activeCamera->m_viewMatrix);

        Scene::displayCallback();

     }

     // Pick a triangle based on click coordinates and turn it black
     void A4Scene::pickTriangle( uint16_t x, uint16_t y ) {
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
         vec3 worldRay = FourDto3d( worldPt - m_activeCamera->m_eye );

         // We now go through every triangle in the scene to see if our ray
         // intersects it
         for ( std::unique_ptr<Entity> &entity : m_entities ) {
             // We are only interested in RecursiveSpheres here
            RecursiveSphere *sphere = dynamic_cast<RecursiveSphere*>( entity.get() );
            if ( sphere == nullptr )
                continue;


             for ( uint16_t triangleIdx = 0; triangleIdx < sphere->m_vertices.size(); triangleIdx += 3 ) {
                 vec3 E = FourDto3d( sphere->m_vertices[ triangleIdx ] );
                 vec3 F = FourDto3d( sphere->m_vertices[ triangleIdx + 1 ] );
                 vec3 G = FourDto3d( sphere->m_vertices[ triangleIdx + 2 ] );

                 // TODO only doing plane test for now
                 float t = rayIntersectsPlane( worldRay, E, F, G );
                 if ( t > 0) {
                    vec3 intersectPt = FourDto3d( m_activeCamera->m_eye ) + t * worldRay;
                    if ( insideTest( E, F, G, intersectPt ) )
                        sphere->blackenTriangle( triangleIdx );
                 }

             }

         }

     }


     float A4Scene::rayIntersectsPlane( vec3 ray, vec3 e, vec3 f, vec3 g ) {
         vec3 N = cross( (f - e), (g - e) );
         vec3 O = FourDto3d( m_activeCamera->m_eye );
         vec3 D = ray;
         float d = dot( -e, N );

         return - ( dot(N, O) + d ) / dot(N,D);

     }


     bool A4Scene::insideTest( vec3 e, vec3 f, vec3 g, vec3 point ) {
         vec3 N = cross( (f - e), (g - e) );
         float dot1 = dot( cross( f - e, point - e ), N );
         float dot2 = dot( cross( g - f, point - f ), N );
         float dot3 = dot( cross( e - g, point - g ), N );

         //std::cout << dot1 << "," << dot2 << "," << dot3 << std::endl;

         return dot1 >= 0 && dot2 >= 0 && dot3 >= 0;

     }


     vec3 A4Scene::FourDto3d( vec4 vector ) {
         return vec3 ( vector.x, vector.y, vector.z );
     }

     */

     // ----- A5 Scene -----
     const std::string A5Scene::VERTEX_LIGHTING_SHADER_NAME = "phong_sun_spot_vert";
     const std::string A5Scene::FRAGMENT_LIGHTING_SHADER_NAME = "phong_sun_spot_frag";
     const std::string A5Scene::FRAGMENT_TEXTURE_SHADER_NAME = "phong_sun_spot_frag_tex";
     
     A5Scene::A5Scene() : Scene() {
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

     void A5Scene::keyboardHandler(unsigned char key, int x, int y) {
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
     
     void A5Scene::specialKeyHandler(int key, int x, int y) {
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

     void A5Scene::displayCallback(){
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

     void A5Scene::timerCallback( int value ) {

         m_lights[0].m_position = RotateZ(0.25) * m_lights[0].m_position;
         m_lights[1].m_position = m_activeCamera->m_eye;
         Scene::timerCallback( value );

     }
}
