#include "cube.hpp"
#include "dice_roller_scene.hpp"
namespace dgfx {
    Cube::Cube(float x, float y, float z, float xrot, float yrot, float zrot) : Object(x,y,z, xrot, yrot, zrot){}

    void Cube::generateGeometry() {
  	vec4 vertices[] = {vec4(-0.5,-0.5,0.5,1.0),vec4(-0.5,0.5,0.5,1.0),vec4(0.5,0.5,0.5,1.0),vec4(0.5,-0.5,0.5,1.0), vec4(-0.5,-0.5,-0.5,1.0),vec4(-0.5,0.5,-0.5,1.0),vec4(0.5,0.5,-0.5,1.0),vec4(0.5,-0.5,-0.5,1.0)};

         makeQuad(1,0,3,2, vertices);  //front
         makeQuad(2,3,7,6, vertices);  //right
         makeQuad(3,0,4,7, vertices);  //bottom
         makeQuad(6,5,1,2, vertices);  //top
         makeQuad(4,5,6,7, vertices);  //back
         makeQuad(5,4,0,1, vertices);  //left

        // Do a basic plane mapping for each side
        for (int i = 0; i < 6; i++ ) {
            m_textureCoords.push_back( vec2(0,0) );
            m_textureCoords.push_back( vec2(1,0) );
            m_textureCoords.push_back( vec2(1,1) );

            m_textureCoords.push_back( vec2(0,0) );
            m_textureCoords.push_back( vec2(1,1) );
            m_textureCoords.push_back( vec2(0,1) );
        }


    }


    void Cube::makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices){
      //Triangle #1
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[b]);
      m_vertices.push_back(vertices[c]);

      //Triangle #2
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[c]);
      m_vertices.push_back(vertices[d]);

    }

    void Cube::textureInit() {
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[2] );
        glBufferData( GL_ARRAY_BUFFER, m_textureCoords.size() * sizeof(vec2), &m_textureCoords[0], GL_STATIC_DRAW );
        GLuint vTexCoordLoc = glGetAttribLocation( m_activeShader, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoordLoc );
        glVertexAttribPointer( vTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        m_textureHandles.resize(2);
        glGenTextures( 2, &m_textureHandles[0] );
        int width = 512, height = 512;

        GLubyte *image = (ppmRead("textures/crate_texture.ppm", &width, &height));
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        
        width = 332, height = 337;

        GLubyte *image2 = (ppmRead("textures/rollsafe.ppm", &width, &height));
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[1] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2 );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


    }

    void Cube::textureDraw() {

        /*
        // We don't want mat properties for textures
        GLuint shaderLoc = glGetUniformLocation( m_activeShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, vec4(1,1,1,1) );
        shaderLoc = glGetUniformLocation( m_activeShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, vec4(1,1,1,1) );
        shaderLoc = glGetUniformLocation( m_activeShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, vec4(1,1,1,1) );
        shaderLoc = glGetUniformLocation( m_activeShader, "Shininess" );
        glUniform1f( shaderLoc, 1.0 );
        */

        if ( m_activeTextureHandle == 0 ) {
            glEnable( GL_TEXTURE_2D );
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );
    
            glUniform1i( glGetAttribLocation( m_activeShader, "textureID" ), 0 );
            return;
        }

        glEnable( GL_TEXTURE_2D );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[1] );

        glUniform1i( glGetAttribLocation( m_activeShader, "textureID" ), 1 );

    }


    void Cube::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }

}
