//
// A GLSL "Hellow World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

//Mesh 0
GLuint buffer;
GLuint VAO;
const int NumVertices = 4;

// Vertices of a unit cube centered at origin, sides aligned with axes
vec2 points[4] = {
    vec2( 0.25, 0.25),
    vec2( 0.75, 0.25),
    vec2( 0.75, 0.75),
    vec2( 0.25, 0.75)
};


// RGBA colors
vec4 blue_opaque = vec4( 0.0, 0.0, 1.0, 1.0 );



//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{


    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader00_v150.glsl", "fshader00_v150.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );

	//Set up VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint color_loc = glGetUniformLocation(program, "color");
	glUniform4fv(color_loc, 1, blue_opaque);

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

	glBindVertexArray(VAO);
	glDrawArrays( GL_TRIANGLE_FAN, 0, NumVertices );

	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
#endif
    glutInitWindowSize( 512, 512 );

    glutCreateWindow( "CS 432 Hello World" );
	m_glewInitAndVersion();


    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutWMCloseFunc(close);

    glutMainLoop();
    return 0;
}

void m_glewInitAndVersion(void)
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

void close(){
	glDeleteBuffers(1, &buffer);  //delete the buffers (free up space on GPU)

}