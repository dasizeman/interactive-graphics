#include <iostream>
#include <string>

#include "2d.h"
#include "daveutils.h"

namespace dgfx {

	/* Polygon */
	
	// VBO Map:
	// 0 - Vertices
    Polygon::Polygon(std::vector<vec3> vertices, std::vector<vec4> colors) :
        m_vertices( vertices ), m_vertexColors( colors ){}


    Polygon::Polygon( std::vector<vec3> vertices ) :
        m_vertices( vertices ){}

    void Polygon::update(std::map<std::string, GLuint>& shaderMap) {
	// Copy vertex data to the VBO
	std::cout << "Polygon::_update()" << std::endl;

    }

    void Polygon::keyboardHandler(unsigned char key, int x, int y){}
    void Polygon::clickHandler(GLint button, GLint state, GLint x, GLint y){}
    
    /* Multicolor Polygon */

    MulticolorPolygon::MulticolorPolygon(std::vector<vec3> vertices, std::vector<vec4> colors) : 
	    Polygon( vertices, colors ){

        m_vertexBuffers.resize(2);
        m_vertexArrays.resize(1);

        // TODO we probably don't want to do this in the constructor, since
        // children classes may not want this shader loaded, but its not a huge
        // deal.
        m_shaderNames.push_back( MULTICOLOR_SHADER_NAME );
    }


	void MulticolorPolygon::init(std::map<std::string, GLuint>& shaderMap) {
        //TODO compile shader and setup VAOS/VBOS, and uniforms
        std::cout << "Polygon::_init()" << std::endl;

        glGenBuffers( 2, &m_vertexBuffers[0] );
        glGenVertexArrays(1, &m_vertexArrays[0]);
        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex data
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW );

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
        // Copy vertex data
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW );

       // Copy color data
       glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
       glBufferData( GL_ARRAY_BUFFER, m_vertexColors.size()*sizeof(vec4), &m_vertexColors[0], GL_STATIC_DRAW );
	}


    void MulticolorPolygon::draw(std::map<std::string, GLuint>& shaderMap) {

    }

    /* SingleColorPolygon */

    SingleColorPolygon::SingleColorPolygon(std::vector<vec3> vertices, vec4 color) :
        Polygon( vertices ), m_color( color ){
        m_vertexBuffers.resize( 1 );
        m_vertexArrays.resize( 1 );

        m_shaderNames.push_back( SINGLE_COLOR_SHADER_NAME );

    }

	void SingleColorPolygon::init(std::map<std::string, GLuint>& shaderMap) {
        glGenBuffers( 1, &m_vertexBuffers[0] );
        glGenVertexArrays(1, &m_vertexArrays[0]);
        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex data
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW );

        GLuint shader = shaderMap[ SINGLE_COLOR_SHADER_NAME ];
        
        GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
        glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        glEnableVertexAttribArray( vPosition );
	}


	void SingleColorPolygon::draw(std::map<std::string, GLuint>& shaderMap) {
        glBindVertexArray( m_vertexArrays[0] );

        // Set the color uniform
        GLuint shader = shaderMap[ SINGLE_COLOR_SHADER_NAME ];
        GLuint color_loc = glGetUniformLocation(shader, "color");
        glUniform4fv(color_loc, 1, m_color);


        glUseProgram( shader );
        glDrawArrays( GL_TRIANGLE_FAN, 0, m_vertices.size() );
	}

	void SingleColorPolygon::update(std::map<std::string, GLuint>& shaderMap)
	{
	}





}
