#include <sstream>

#include "scene.h"
#include "daveutils.h"
#include "2d.h"

namespace dgfx {

    const std::string Scene::DEFAULT_WINDOW_NAME = "CS432";
    const std::string Scene::SHADER_PATH = "shaders/";
    const int Scene::DEFAULT_WINDOW_WIDTH = 500, Scene::DEFAULT_WINDOW_HEIGHT = 500;
    

    Scene::Scene() : 
        Scene::Scene( DEFAULT_WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT){}
    
    Scene::Scene(std::string windowName, int width, int height) : 
        m_screenWidth( width ), m_screenHeight( height ){
        init( windowName, width, height );
    }

    Scene& Scene::getInstance() {
	static Scene instance;
	return instance;
    }

    void Scene::init(std::string windowName, int width, int height) {
#ifdef __APPLE__
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
#endif
	glutInitWindowSize( width, height );

	glutCreateWindow( windowName.c_str() );
	glewInitAndVersion();


	// Assign event callbacks
	glutDisplayFunc( &Scene::display_callback_wrapper );
	glutMouseFunc( &Scene::click_callback_wrapper );
	glutKeyboardFunc( &Scene::keyboard_callback_wrapper );
    glutWMCloseFunc( &Scene::close_handler );


    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    }


    void Scene::addEntity(std::unique_ptr<Entity> entity) {
	// Load all of the entity's shaders
	for ( std::string shaderName : entity->getShaderNames() )
	    addShader( shaderName );

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
	glutMainLoop();
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

    void Scene::clickHandler(GLint button, GLint state, GLint x, GLint y) {
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->clickHandler( button, state, x, y);

        //std::cout << x << "," << y << std::endl;
        vec4 blue_opaque = vec4( 0.0, 0.0, 1.0, 1.0 );
        vec4 red_opaque = vec4( 1.0, 0.0, 0.0, 1.0 );

        if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
            auto squarePts = daveutils::generateSquareVertices( x ,y, 20 );
            daveutils::convertToCameraCoordinates( squarePts, m_screenWidth, m_screenHeight );
            std::cout << daveutils::printVector( squarePts ) << std::endl;
            addEntity( std::unique_ptr<Entity>(new SingleColorPolygon( squarePts, blue_opaque ) ));
            std::cout << m_entities.size() << std::endl;
            glutPostRedisplay();
        }
        

    }

    void Scene::displayCallback() {
        glClear( GL_COLOR_BUFFER_BIT );
        std::cout << "Display called!" << std::endl;


        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->draw( m_shaderMap );

        //glutSwapBuffers();
        glFlush();
    }

    void Scene::timerCallback( int value ) {
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->update( m_shaderMap );
    }

    void Scene::cleanup() {
        //for ( std::unique_ptr<Entity> &entity : m_entities ) {
        //    auto ptr = entity.release();
        //    delete( ptr );
        //}

    }

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
	    Scene::getInstance().displayCallback();
    }

     void Scene::timer_callback_wrapper( int value ) {
	Scene::getInstance().timerCallback( value );
     }

     void Scene::keyboard_callback_wrapper(unsigned char key, int x, int y) {
	Scene::getInstance().keyboardHandler( key, x, y );
     }

     void Scene::click_callback_wrapper(GLint button, GLint state, GLint x, GLint y) {
	 Scene::getInstance().clickHandler( button, state, x, y );
     }

	void Scene::close_handler() {
        Scene::getInstance().cleanup(); 
    }
    
}
