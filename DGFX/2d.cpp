#include <iostream>
#include "2d.h"

namespace dgfx {

	/* Polygon */
	
	// VBO Map:
	// 0 - Vertices
    Polygon::Polygon(std::vector<vec2> vertices, std::vector<vec4> colors) : 
	    m_vertices(vertices),
	    m_vertexColors(colors){
		init();
		m_vertexArrays.resize(1);
		m_vertexBuffers.resize(2);
	    }

    Polygon::~Polygon() {
	glDeleteBuffers( m_vertexBuffers.size(), &m_vertexBuffers[0] );	
    }

    void Polygon::init() {
	//TODO compile shader and setup VAOS/VBOS, and uniforms
	std::cout << "Polygon::_init()" << std::endl;

	glGenBuffers( 2, &m_vertexBuffers[0] );
	glGenVertexArrays(1, &m_vertexArrays[0]);
	glBindVertexArray( m_vertexArrays[0] );
	
    }

    void Polygon::update(const std::map<std::string, GLuint>& shaderMap) {
	// Copy vertex data to the VBO
	std::cout << "Polygon::_update()" << std::endl;

    }

    void Polygon::draw(const std::map<std::string, GLuint>& shaderMap) {

    }
    void Polygon::keyboardHandler(unsigned char key, int x, int y){}
    void Polygon::clickHandler(GLint button, GLint state, GLint x, GLint y){}
    
    void Polygon::updateBuffers() {
	// Copy vertex data
       glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );	  
       glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vec2), &m_vertices[0], GL_STATIC_DRAW );

       // Copy color data
       glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
       glBufferData( GL_ARRAY_BUFFER, m_vertexColors.size()*sizeof(vec4), &m_vertexColors[0], GL_STATIC_DRAW );
    }
}
