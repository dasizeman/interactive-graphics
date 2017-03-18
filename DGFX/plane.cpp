#include "plane.hpp"
#include "dice_roller_scene.hpp"
namespace dgfx {
    Plane::Plane( float x, float y, float z, float xrot,float yrot, float zrot, float sideLength) :
        Object(x,y,z,xrot,yrot,zrot) {
            m_sideLength = sideLength;
        }


    void Plane::generateGeometry() {
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); // TL
        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BR
        m_vertices.push_back( vec4( -m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BL

        m_vertices.push_back( vec4( m_sideLength / 2, 0, m_sideLength / 2, 1 ) ); // TR
        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BR
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); // TL

        // Do a basic plane mapping for each side
        for (int i = 0; i < 6; i++ ) {
            m_textureCoords.push_back( vec2(0,1) );
            m_textureCoords.push_back( vec2(1,0) );
            m_textureCoords.push_back( vec2(0,0) );

            m_textureCoords.push_back( vec2(1,1) );
            m_textureCoords.push_back( vec2(1,0) );
            m_textureCoords.push_back( vec2(0,1) );
        }


    }

    void Plane::textureInit() {
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[2] );
        glBufferData( GL_ARRAY_BUFFER, m_textureCoords.size() * sizeof(vec2), &m_textureCoords[0], GL_STATIC_DRAW );
        GLuint vTexCoordLoc = glGetAttribLocation( m_activeShader, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoordLoc );
        glVertexAttribPointer( vTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        m_textureHandles.resize(2);
        glGenTextures( 2, &m_textureHandles[0] );
        int width = 512, height = 512;

        GLubyte *image = (ppmRead("textures/grass.ppm", &width, &height));
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    }

    void Plane::textureDraw() {

        glEnable( GL_TEXTURE_2D );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );

        glUniform1i( glGetAttribLocation( m_activeShader, "textureID" ), 0 );

    }


    void Plane::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }

};
