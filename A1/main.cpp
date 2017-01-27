//
// A GLSL "Hellow World"
// Display a blue square
//

#include <vector>
#include <cmath>
#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "daveutils.h"
void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

//Mesh 0
GLuint singleColorProgram, variableColorProgram;
GLuint singleColorVAO, variableColorVAO;
const int NumVertices = 7;

// Vertices of a unit cube centered at origin, sides aligned with axes
std::vector<vec2> points = {
    vec2( 0.25, 0.25),
    vec2( 0.75, 0.25),
    vec2( 0.75, 0.75),
    vec2( 0.25, 0.75),
    vec2( 0.0, 0.25),
    vec2( 0.5, 0.25),
    vec2( 0.5, 0.75)
};

std::vector<vec2> circlePts;


// RGBA colors
vec4 blue_opaque = vec4( 0.0, 0.0, 1.0, 1.0 );
vec4 red_opaque = vec4( 1.0, 0.0, 0.0, 1.0 );

std::vector<vec2> generateCirclePoints(vec2 startPt, float radius) {
    std::vector<vec2> result;
    int numSteps = 100;
    float stepSize = (2*M_PI) / numSteps;

    // Add the center point
    result.push_back(startPt);

    float theta = 0;
    while (theta < 2*M_PI) {
        result.push_back(vec2(startPt.x + radius*cos(theta), startPt.y + radius*sin(theta)));
        theta += stepSize;
    }

    // Add the initial point again
    result.push_back(result[1]);

    return result;

}

std::vector<vec4> generateColors(int n) {
    std::vector<vec4> result;
    for (int i = 0; i < n; i++ ) {
        result.push_back(vec4(daveutils::randomFloat(0,1.0), 
                daveutils::randomFloat(0,1.0),
                daveutils::randomFloat(0,1.0),
                1.0));
    }

    return result;
}



//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{

    // Generate circle points
    circlePts = generateCirclePoints(vec2(-0.25,0), 0.25);
    points.insert(points.end(), circlePts.begin(), circlePts.end());

    // Generate circle colors
    std::vector<vec4> circleColors = generateColors(circlePts.size());

    // Load shaders
    singleColorProgram = InitShader( "a1.vert", "a1_uniform.frag" );
    variableColorProgram = InitShader( "a1_colorpassthrough.vert", "a1_variable.frag");


    // ----Single color VAO setup-----
    // Generate a VAO
    glGenVertexArrays( 1, &singleColorVAO );
    glBindVertexArray( singleColorVAO );

    // Make a buffer for the vertices
    GLuint singleColorPointBuffer;
    glGenBuffers( 1, &singleColorPointBuffer );

    // Copy the vertex position data
    glBindBuffer( GL_ARRAY_BUFFER, singleColorPointBuffer );
    glBufferData( GL_ARRAY_BUFFER, points.size()*sizeof(vec2), &points[0], GL_STATIC_DRAW );

    // Tell OpenGL how to read the vertex data
    GLuint vPositionSingleColor = glGetAttribLocation( singleColorProgram, "vPosition" );
    glVertexAttribPointer( vPositionSingleColor, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPositionSingleColor );

    // -----Variable color VAO setup-----
    glGenVertexArrays( 1, &variableColorVAO );
    glBindVertexArray( variableColorVAO );
    
    // Copy the vertex position data
    GLuint variableColorPointBuffer;
    glGenBuffers( 1, &variableColorPointBuffer);

    glBindBuffer( GL_ARRAY_BUFFER, variableColorPointBuffer );
    glBufferData( GL_ARRAY_BUFFER, circlePts.size()*sizeof(vec2), &circlePts[0], GL_STATIC_DRAW);
    GLuint vPositionVariableColor = glGetAttribLocation( variableColorProgram, "vPosition" );
    glVertexAttribPointer( vPositionSingleColor, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPositionVariableColor );

    // Copy the vertex color data
    GLuint variableColorColorBuffer;
    glGenBuffers( 1, &variableColorColorBuffer );

    glBindBuffer( GL_ARRAY_BUFFER, variableColorColorBuffer );
    glBufferData( GL_ARRAY_BUFFER, circleColors.size()*sizeof(vec4), &circleColors[0], GL_STATIC_DRAW );
    GLuint cPosition = glGetAttribLocation( variableColorProgram, "vColorIn" );
    glVertexAttribPointer( cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( cPosition );


    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

    GLuint color_loc = glGetUniformLocation(singleColorProgram, "color");
    glBindVertexArray(singleColorVAO);
    glUseProgram( singleColorProgram );
    glUniform4fv(color_loc, 1, red_opaque);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

    glUniform4fv(color_loc, 1, blue_opaque);
    glDrawArrays( GL_TRIANGLE_FAN, 4, 3 );

    glBindVertexArray( variableColorVAO );
    glUseProgram( variableColorProgram );
    glDrawArrays( GL_TRIANGLE_FAN, 0, circlePts.size());

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
	//glDeleteBuffers(1, &singleColorPointBuffer);  //delete the buffers (free up space on GPU)

}
