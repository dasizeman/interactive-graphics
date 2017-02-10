#include <iostream>
#include <string>

#include "geometry.hpp"
#include "daveutils.hpp"

namespace dgfx {

	/* Polygon */
	
	// VBO Map:
	// 0 - Vertices
    Polygon::Polygon(std::vector<vec4> vertices, std::vector<vec4> colors, float x, float y) :
        m_vertices( vertices ), 
        m_vertexColors( colors ),
        m_x(x),
        m_y(y){}


    Polygon::Polygon( std::vector<vec4> vertices, float x, float y ) :
        Polygon( vertices, std::vector<vec4>(), x, y){}


    void Polygon::keyboardHandler(unsigned char key, int x, int y){ 
    }

    void Polygon::clickHandler(GLint button, GLint state, GLint x, GLint y){}
    void Polygon::specialKeyHandler(int key, int x, int y) {

    }


    mat4 Polygon::calculateModelMatrix ( float theta ) {
        // Translate to origin
        mat4 t1 (
                vec4(1, 0, 0,-m_x),
                vec4(0, 1, 0, -m_y),
                vec4(0,0,1,0),
                vec4(0, 0,0, 1));


        // Rotate
        mat4 rot (
                vec4(cos(m_theta), -sin(m_theta), 0,0),
               vec4( sin(m_theta), cos(m_theta), 0,0),
                vec4(0, 0,1, 0),
                vec4(0, 0,0, 1));


        // Translate back
        mat4 t2 (
                vec4(1, 0, 0, m_x),
                vec4(0, 1, 0, m_y),
                vec4(0, 0, 1, 0),
                vec4(0, 0,0, 1));


        mat4 result = t2*rot*t1;
        return result;
    }
    
    /* Multicolor Polygon */

    MulticolorPolygon::MulticolorPolygon(std::vector<vec4> vertices, std::vector<vec4> colors, float x, float y) : 
	    Polygon( vertices, colors, x, y ){

        m_vertexBuffers.resize(2);
        m_vertexArrays.resize(1);

        m_shaderNames.push_back(MULTICOLOR_SHADER_NAME);


    }


	void MulticolorPolygon::init(std::map<std::string, GLuint>& shaderMap) {
            //TODO compile shader and setup VAOS/VBOS, and uniforms
            std::cout << "Polygon::_init()" << std::endl;

            glGenBuffers( 2, &m_vertexBuffers[0] );
            glGenVertexArrays(1, &m_vertexArrays[0]);
            glBindVertexArray( m_vertexArrays[0] );

            // Copy vertex data
            glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
            glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );

            GLuint shader = shaderMap[ MULTICOLOR_SHADER_NAME ];
            
            GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
            glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
            glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
            glEnableVertexAttribArray( vPosition );


            GLuint cPosition = glGetAttribLocation( shader, "vColorIn" );
            glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
            glVertexAttribPointer( cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
            glEnableVertexAttribArray( cPosition );
	}

	void MulticolorPolygon::update(std::map<std::string, GLuint>& shaderMap) {
           // Copy color data
           glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
           glBufferData( GL_ARRAY_BUFFER, m_vertexColors.size()*sizeof(vec4), &m_vertexColors[0], GL_STATIC_DRAW );
	}


    void MulticolorPolygon::draw(std::map<std::string, GLuint>& shaderMap) {
            GLuint shader = shaderMap[ MULTICOLOR_SHADER_NAME ];
            glUseProgram( shader );

            // Set the model matrix uniform
            GLuint matrix_loc = glGetUniformLocation( shader, "model_view" );
            glUniformMatrix4fv(matrix_loc,1, GL_TRUE,m_modelMatrix);

            glDrawArrays( GL_TRIANGLE_FAN, 0, m_vertices.size() );

    }


    /* SingleColorPolygon */

    SingleColorPolygon::SingleColorPolygon(std::vector<vec4> vertices, vec4 color, float x, float y) :
        Polygon( vertices, x, y ), m_color( color ), m_currentColor( color ){
        m_vertexBuffers.resize( 1 );
        m_vertexArrays.resize( 1 );

        m_shaderNames.push_back(SINGLE_COLOR_SHADER_NAME);

    }

	void SingleColorPolygon::init(std::map<std::string, GLuint>& shaderMap) {
            glGenBuffers( 1, &m_vertexBuffers[0] );
            glGenVertexArrays(1, &m_vertexArrays[0]);
            glBindVertexArray( m_vertexArrays[0] );

            // Copy vertex data
            glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
            glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );

            GLuint shader = shaderMap[ SINGLE_COLOR_SHADER_NAME ];
            
            GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
            glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
            glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
            glEnableVertexAttribArray( vPosition );
	}


	void SingleColorPolygon::draw(std::map<std::string, GLuint>& shaderMap) {
            glBindVertexArray( m_vertexArrays[0] );

            // Set the color uniform
            GLuint shader = shaderMap[ SINGLE_COLOR_SHADER_NAME ];
            GLuint color_loc = glGetUniformLocation(shader, "color");
            glUniform4fv(color_loc, 1, m_currentColor);

            // Set the model matrix uniform
            GLuint matrix_loc = glGetUniformLocation( shader, "model_view" );
            glUniformMatrix4fv(matrix_loc,1, GL_TRUE,m_modelMatrix);



            glUseProgram( shader );
            glDrawArrays( GL_TRIANGLE_FAN, 0, m_vertices.size() );
	}


	void SingleColorPolygon::update(std::map<std::string, GLuint>& shaderMap) {
        if( !m_doAnimation )
            return;

        if( m_theta >= 2*M_PI )
            m_theta = 0;

        m_theta += (2*M_PI)/100;

        m_currentColor = m_color * ((sin(m_theta) + 1.0) / 2.0);

        m_modelMatrix = calculateModelMatrix ( m_theta );
	}

    // -------------------------------------
    // 3D 
    // ------------------------------------
    //
    //
        FlatSquareAttributeGenerator::FlatSquareAttributeGenerator(float sideLength) :
            m_sideLength(sideLength){}


    void FlatSquareAttributeGenerator::generate( 
            std::vector<vec4> &vertices,
            std::vector<GLuint> &elements,
            std::vector<vec4> &colors )  {

        float offset = m_sideLength / 2;
        float z = offset;

        // First we will define the 8 unique vertices on the front and back
        
        // 


    }

    

    Model::Model( AttributeGenerator generator,
                     float x,
                     float y, 
                     float z) {

    }

    void Model::translate (float x, float y, float z) {

    }
    void Model::rotate    (float x, float y, float z) {

    }


    // Called by the scene to set up GL data structures
    void Model::init(std::map<std::string, GLuint>& shaderMap) {

    }
    
    // Called by the scene to update GL state based on internal state
    void Model::update(std::map<std::string, GLuint>& shaderMap) {

    }

    // Called by the scene to draw the object
    void Model::draw(std::map<std::string, GLuint>& shaderMap) {

    }

    // Called by the scene on an event
    void Model::keyboardHandler(unsigned char key, int x, int y) {

    }
    void Model::clickHandler(GLint button, GLint state, GLint x, GLint y) {

    }
    void Model::specialKeyHandler(int key, int x, int y) {

    }

    void Model::setShaders() {

    }
}
