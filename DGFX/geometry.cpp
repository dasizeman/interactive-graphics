#include <iostream>
#include <string>

#include "geometry.hpp"
#include "daveutils.hpp"
#include "scene.hpp"

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
        void Model::generate() {

            m_vertices= daveutils::generateNGon(m_n, m_size, m_depth);
            for (int i = 0; i < m_vertices.size(); i++) {
                vec4 color;
                color.x = daveutils::randomFloat( 0, 1);
                color.y = daveutils::randomFloat( 0, 1);
                color.z = daveutils::randomFloat( 0, 1);
                color.w = 1;

                m_colors.push_back( color );

            }

            // String the elements such that all triangles are outward facing.
            // We'll start with the front and back faces
            m_elements.resize( 2 * m_n);
            for (int i = 0; i < m_n; i++) {
                m_elements[i] = i;
            }
            for (int i = 0; i < m_n; i++) {
                m_elements[ i + m_n ] = 2 * m_n - i - 1;
            }

            vec3 E = daveutils::FourDto3d(m_vertices[ m_elements[0] ]);
            vec3 F = daveutils::FourDto3d(m_vertices[ m_elements[1] ]);
            vec3 G = daveutils::FourDto3d(m_vertices[ m_elements[m_n - 1] ]);
            vec3 normal = normalize( cross( F - E, G - E )); 
            for ( int i = 0; i < m_n; i++)
                m_normals.push_back(normal);

            for ( int i = 0; i < m_n; i++)
                m_normals.push_back(-normal);
           // Now we connect the front and back faces by specifying the corners
           // of a quad connecting each side of the front and back
           for (int frontIdx = 0; frontIdx < m_n; frontIdx++ ) {

               // Special case for last side
               if (frontIdx == m_n - 1) {
                   m_elements.push_back( frontIdx );
                   m_elements.push_back( frontIdx + m_n );
                   m_elements.push_back( frontIdx + 1 );
                   m_elements.push_back( 0 );

                   E = daveutils::FourDto3d( m_vertices[ frontIdx ] );
                   F = daveutils::FourDto3d( m_vertices[ frontIdx  + m_n ] );
                   G = daveutils::FourDto3d( m_vertices[ 0 ] );

                   normal = normalize( cross( F - E, G - E )); 
                   for (int i = 0; i < 4; i++)
                       m_normals.push_back( normal );
                   continue;
               }
                m_elements.push_back( frontIdx );
                m_elements.push_back( frontIdx + m_n );
                m_elements.push_back( frontIdx + m_n + 1 );
                m_elements.push_back( frontIdx + 1 );


                E = daveutils::FourDto3d( m_vertices[ frontIdx ] );
                F = daveutils::FourDto3d( m_vertices[ frontIdx + m_n ] );
                G = daveutils::FourDto3d( m_vertices[ frontIdx + 1 ] );
                normal = normalize( cross( F - E, G - E )); 

                for(int i = 0; i < 4; i++)
                    m_normals.push_back( normal);

           } 
    }


    Model::Model( float x,
                  float y,
                  float z ) :
        m_x( x ),
        m_y( y ),
        m_z( z ){

        m_vertexBuffers.resize( 3 );
        m_vertexArrays.resize( 2 );
    }

    Model::Model(float x,
                 float y, 
                 float z,
                 uint16_t n,
                 float size,
                 float depth) : 
                 Model( x, y, z ) {

        m_n = n;
        m_size = size;
        m_depth = depth;

        // TODO Bad design note, this Model class should really be a Polyhedron,
        // and there should be another common base class
        generate();
    }

    void Model::translate (float x, float y, float z) {

    }
    void Model::rotate    (float x, float y, float z) {

    }

    void Model::calculateNormals() {
        //TODO populate m_normals
        for (int i = 0; i < m_vertices.size(); i += 3 ) {
            int vidx = 0;
            vec3 E( m_vertices[i + vidx].x, m_vertices[i + vidx].y, m_vertices[i + vidx].z );
            vidx++;
            vec3 F( m_vertices[i + vidx].x, m_vertices[i + vidx].y, m_vertices[i + vidx].z );
            vidx++;
            vec3 G( m_vertices[i + vidx].x, m_vertices[i + vidx].y, m_vertices[i + vidx].z );

            vec3 normal = normalize( cross( F - E, G - E )); 
            m_normals.push_back( normal );
            m_normals.push_back( normal );
            m_normals.push_back( normal );
        }
    }


    // Called by the scene to set up GL data structures
    void Model::init(std::map<std::string, GLuint>& shaderMap) {

        GLuint mainShader = shaderMap[ Scene::FLAT_3D_SHADER_NAME ];

        glGenBuffers(3, &m_vertexBuffers[0]);
        glGenVertexArrays( 2, &m_vertexArrays[0]);

        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex position data and set up the attribute pointers
        
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[0] );
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );
        GLuint vPositionLoc = glGetAttribLocation( mainShader, "vPosition" );
        glEnableVertexAttribArray( vPositionLoc );
        glVertexAttribPointer( vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Do the same for color data
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
        glBufferData( GL_ARRAY_BUFFER, m_colors.size() * sizeof(vec4), &m_colors[0], GL_STATIC_DRAW );
        GLuint vColorInLoc = glGetAttribLocation( mainShader, "vColorIn" );
        glEnableVertexAttribArray( vColorInLoc );
        glVertexAttribPointer( vColorInLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Copy over the element data
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[2] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_elements.size() * sizeof(GLuint), &m_elements[0], GL_STATIC_DRAW );

        // Set up the wireframe shader
        ///*
        GLuint wireframeShader = shaderMap[ Scene::WIREFRAME_SHADER_NAME ];
        glBindVertexArray( m_vertexArrays[1] );
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[0] );
        vPositionLoc = glGetAttribLocation( wireframeShader, "vPosition" );
        glEnableVertexAttribArray( vPositionLoc );
        glVertexAttribPointer( vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[2] );
        //*/
        

        // Generate the model matrix.  We also need to apply it to our in-memory
        // vertex data for the sake of collision detection
        // TODO WARNING!! This is not the right way to do this, as if we ever
        // copy the vertex data to the gpu this transformation will stick!!
        mat4 modelMatrix = Translate(m_x, m_y, m_z );
        for ( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 oldVertex = m_vertices[ i ];
            m_vertices[ i ] = modelMatrix * oldVertex;
        }



    }
    
    // Called by the scene to update GL state based on internal state
    void Model::update(std::map<std::string, GLuint>& shaderMap) {
        if (!m_doAnimation)
            return;

        if ( m_yRot >= 360)
            m_yRot = 0;
        m_yRot += 1;

        // Generate the model matrix.  We also need to apply it to our in-memory
        // vertex data for the sake of collision detection
        // TODO WARNING!! This is not the right way to do this, as if we ever
        // copy the vertex data to the gpu this transformation will stick!!
        mat4 modelMatrix = Translate(m_x, m_y, m_z ) * 
                           RotateY( 1 ) *
                           Translate( -m_x, -m_y, -m_z );
        for ( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 oldVertex = m_vertices[ i ];
            m_vertices[ i ] = modelMatrix * oldVertex;
        }

    }

    // Called by the scene to draw the object
    void Model::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ Scene::FLAT_3D_SHADER_NAME ];
        GLuint wireframeShader = shaderMap[ Scene::WIREFRAME_SHADER_NAME ];
        // Set the model matrix uniform

        //TODO generate model matrix here based on the current parameters of the
        //model

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, Translate( m_x, m_y, m_z ) * RotateY(m_yRot));

        glUseProgram( wireframeShader );
        glEnable( GL_LINE_SMOOTH );
        glBindVertexArray( m_vertexArrays[1] );
        GLuint wireframeModelMatrix = glGetUniformLocation( wireframeShader, "model_matrix" );
        glUniformMatrix4fv(wireframeModelMatrix,1, GL_TRUE, Translate( m_x, m_y, m_z) * RotateY(m_yRot));
        GLuint wireframeColorUniformLoc = glGetUniformLocation( wireframeShader, "wireColor" ); 
        glUniform4fv( wireframeColorUniformLoc, 1, vec4(0, 0, 0, 1) ); 

        uint64_t elementOffset = 0;

        // Draw the front and back faces
        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        glDrawElements( GL_TRIANGLE_FAN, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );


        glUseProgram( wireframeShader );
        glBindVertexArray( m_vertexArrays[1] );
        glDrawElements( GL_LINE_LOOP, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );

        elementOffset += m_n;

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        glDrawElements( GL_TRIANGLE_FAN, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );


        glUseProgram( wireframeShader );
        glBindVertexArray( m_vertexArrays[1] );
        glDrawElements( GL_LINE_LOOP, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );

        elementOffset += m_n;

        // Draw each connecting quad
        for (int i = 0; i < m_n; i++ ) {
            glUseProgram( mainShader );
            glBindVertexArray( m_vertexArrays[0] );
            glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );
            
            glUseProgram( wireframeShader );
            glBindVertexArray( m_vertexArrays[1] );
            glDrawElements( GL_LINE_LOOP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );

            elementOffset += 4;
        }

    }

    // Called by the scene on an event
    void Model::keyboardHandler(unsigned char key, int x, int y) {

    }
    void Model::clickHandler(GLint button, GLint state, GLint x, GLint y) {

    }
    void Model::specialKeyHandler(int key, int x, int y) {

    }

    // ----- RecursiveSphere -----
    
    RecursiveSphere::RecursiveSphere( float x,
                         float y,
                         float z,
                         uint16_t numRecursions ) : 
                         Model( x, y, z ){
        m_recursionDepth = numRecursions;
        generate();
    }

    void RecursiveSphere::init(std::map<std::string, GLuint>& shaderMap) {
        Model::init( shaderMap );
    }


    point4 RecursiveSphere::unit( const point4 &p ) {
        float len = p.x*p.x + p.y*p.y + p.z*p.z;
        
        point4 t;
        if ( len > DivideByZeroTolerance ) {
            t = p / sqrt(len);
            t.w = 1.0;
        }

        return t;

    }

    void RecursiveSphere::triangle( const point4& a, 
                       const point4& b, 
                       const point4& c ) {

        m_vertices.push_back( a );
        m_vertices.push_back( b );
        m_vertices.push_back( c );
    }

    void RecursiveSphere::tetrahedron( int count ) {
        point4 v[4] = {
            vec4( 0.0, 0.0, 1.0, 1.0 ),
            vec4( 0.0, 0.942809, -0.333333, 1.0 ),
            vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
            vec4( 0.816497, -0.471405, -0.333333, 1.0 )
        };

        divide_triangle( v[0], v[1], v[2], count );
        divide_triangle( v[3], v[2], v[1], count );
        divide_triangle( v[0], v[3], v[1], count );
        divide_triangle( v[0], v[2], v[3], count );

    }

    void RecursiveSphere::divide_triangle( const point4 &a, 
                              const point4 &b,
                              const point4 &c,
                              int count ) {
        if ( count > 0 ) {
            point4 v1 = unit( a + b );
            point4 v2 = unit( a + c );
            point4 v3 = unit( b + c );
            divide_triangle(  a, v1, v2, count - 1 );
            divide_triangle(  c, v2, v3, count - 1 );
            divide_triangle(  b, v3, v1, count - 1 );
            divide_triangle( v1, v3, v2, count - 1 );
        }
        else {
            triangle( a, b, c );
        }

    }

    void RecursiveSphere::generate() {
        tetrahedron( m_recursionDepth );
        for( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 color;
            color.x = daveutils::randomFloat( 0, 1 );
            color.y = daveutils::randomFloat( 0, 1 );
            color.z = daveutils::randomFloat( 0, 1 );
            color.w = 1;

            m_colors.push_back( color );
        }

    }


        // Called by the scene to draw the object
    void RecursiveSphere::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ Scene::FLAT_3D_SHADER_NAME ];
        GLuint wireframeShader = shaderMap[ Scene::WIREFRAME_SHADER_NAME ];

        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot);

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() );

    }


    void RecursiveSphere::blackenTriangle( uint16_t triangleIdx ) {
        // Modify the color vertex attributes for the triangle at the index
        vec4 black( 0, 0, 0, 1 );
        vec4 blacks[] = {black, black, black};
        glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffers[1] );
        glBufferSubData( GL_ARRAY_BUFFER, triangleIdx * sizeof(vec4), 3*sizeof( vec4 ), &blacks );

    }

    // ----- FloorPlane -----
    
    FloorPlane::FloorPlane( uint16_t sideLength, vec4 color) : 
        Model( 0, 0, 0 ),
        m_sideLength( sideLength ),
        m_color ( color ) {
        
        generate();
    }

    void FloorPlane::generate() {
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); 
        m_colors.push_back( m_color );

        m_vertices.push_back( vec4( -m_sideLength / 2, 0, -m_sideLength / 2, 1 ) );
        m_colors.push_back( m_color );

        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) );
        m_colors.push_back( m_color );

        m_vertices.push_back( vec4( m_sideLength / 2, 0, m_sideLength / 2, 1 ) );
        m_colors.push_back( m_color );

        vec3 E = daveutils::FourDto3d(m_vertices[0]);
        vec3 F = daveutils::FourDto3d(m_vertices[1]);
        vec3 G = daveutils::FourDto3d(m_vertices[2]);
        vec3 normal = -normalize( cross( F - E, G - E )); 
        m_normals.push_back( normal );
        m_normals.push_back( normal );
        m_normals.push_back( normal );
        m_normals.push_back( normal );
    }


    // Called by the scene to draw the object
    void FloorPlane::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ Scene::FLAT_3D_SHADER_NAME ];
        GLuint wireframeShader = shaderMap[ Scene::WIREFRAME_SHADER_NAME ];

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, Translate( m_x, m_y, m_z ) * RotateY(m_yRot));

        glDrawArrays( GL_TRIANGLE_FAN, 0, m_vertices.size() );

    }

    // TODO yes we really need some sort of "lighted" base class...
    // ----- LightedPolyhedron -----
    LightedPolyhedron::LightedPolyhedron( float x,
             float y, 
             float z,
             uint16_t n,
             float size, 
             float depth) : Model(x,y,z,n,size,depth) {


        // TODO we are just going to use some arbitrary material values for now
        m_ambient = vec4( 1.0, 0.0, 1.0, 1.0 );
        m_diffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
        m_specular = vec4( 1.0, 0.0, 1.0, 1.0 );
        m_shininess = 5.0;
        std::cout << "vertices: " << m_vertices.size() << " normals: " << m_normals.size() << std::endl;
    }

    // Called by the scene to draw the object
    void LightedPolyhedron::init(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];

        glGenBuffers(3, &m_vertexBuffers[0]);
        glGenVertexArrays( 1, &m_vertexArrays[0]);

        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex position data and set up the attribute pointers
        
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[0] );
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );
        GLuint vPositionLoc = glGetAttribLocation( mainShader, "vPosition" );
        glEnableVertexAttribArray( vPositionLoc );
        glVertexAttribPointer( vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Copy the normals
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[1] );
        glBufferData( GL_ARRAY_BUFFER, m_normals.size() * sizeof(vec3), &m_normals[0], GL_STATIC_DRAW );
        GLuint vNormalLoc = glGetAttribLocation( mainShader, "vNormal" );
        glEnableVertexAttribArray( vNormalLoc );
        glVertexAttribPointer( vNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Copy over the element data
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[2] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_elements.size() * sizeof(GLuint), &m_elements[0], GL_STATIC_DRAW );



        // Generate the model matrix.  We also need to apply it to our in-memory
        // vertex data for the sake of collision detection
        // TODO WARNING!! This is not the right way to do this, as if we ever
        // copy the vertex data to the gpu this transformation will stick!!
        mat4 modelMatrix = Translate(m_x, m_y, m_z );
        for ( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 oldVertex = m_vertices[ i ];
            m_vertices[ i ] = modelMatrix * oldVertex;
        }

    }

    void LightedPolyhedron::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];

        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot);

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        // Set material property uniforms
        GLuint shaderLoc = glGetUniformLocation( mainShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, m_ambient );
        shaderLoc = glGetUniformLocation( mainShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, m_specular );
        shaderLoc = glGetUniformLocation( mainShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, m_diffuse );
        shaderLoc = glGetUniformLocation( mainShader, "Shininess" );
        glUniform1f( shaderLoc, m_shininess );

        uint64_t elementOffset = 0;

        // Draw the front and back faces
        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        glDrawElements( GL_TRIANGLE_FAN, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );


        elementOffset += m_n;

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );
        glDrawElements( GL_TRIANGLE_FAN, m_n, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );


        elementOffset += m_n;

        // Draw each connecting quad
        for (int i = 0; i < m_n; i++ ) {
            glUseProgram( mainShader );
            glBindVertexArray( m_vertexArrays[0] );
            glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(elementOffset * sizeof(GLuint)) );

            elementOffset += 4;
        }

    }


    // ----- LightedRecursiveSphere ------
    LightedRecursiveSphere::LightedRecursiveSphere( float x,
                     float y,
                     float z,
                     uint16_t numRecursions ) : RecursiveSphere( x,y,z,numRecursions){
        // Calculate the normals
        // TODO: TERRIBLE DESIGN this is implemented to work for polyhedrons but
        // it doesn't.  This is a result of the crappy base model class design,
        // if anything, the default should work for polyhedrons and we should
        // override it for the sphere but I'm in a rush
        Model::calculateNormals();
        std::cout << "vertices: " << m_vertices.size() << " normals: " << m_normals.size() << std::endl;

        // TODO we are just going to use some arbitrary material values for now
        m_ambient = vec4( 1.0, 0.0, 1.0, 1.0 );
        m_diffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
        m_specular = vec4( 1.0, 0.0, 1.0, 1.0 );
        m_shininess = 5.0;

    }

     void LightedRecursiveSphere::init(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];

        glGenBuffers(2, &m_vertexBuffers[0]);
        glGenVertexArrays( 1, &m_vertexArrays[0]);

        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex position data and set up the attribute pointers
        
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[0] );
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );
        GLuint vPositionLoc = glGetAttribLocation( mainShader, "vPosition" );
        glEnableVertexAttribArray( vPositionLoc );
        glVertexAttribPointer( vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Copy the normals
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[1] );
        glBufferData( GL_ARRAY_BUFFER, m_normals.size() * sizeof(vec3), &m_normals[0], GL_STATIC_DRAW );
        GLuint vNormalLoc = glGetAttribLocation( mainShader, "vNormal" );
        glEnableVertexAttribArray( vNormalLoc );
        glVertexAttribPointer( vNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );



        // Generate the model matrix.  We also need to apply it to our in-memory
        // vertex data for the sake of collision detection
        // TODO WARNING!! This is not the right way to do this, as if we ever
        // copy the vertex data to the gpu this transformation will stick!!
        mat4 modelMatrix = Translate(m_x, m_y, m_z );
        for ( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 oldVertex = m_vertices[ i ];
            m_vertices[ i ] = modelMatrix * oldVertex;
        }

     }
     void LightedRecursiveSphere::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];

        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot);

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        // Set material property uniforms
        GLuint shaderLoc = glGetUniformLocation( mainShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, m_ambient );
        shaderLoc = glGetUniformLocation( mainShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, m_specular );
        shaderLoc = glGetUniformLocation( mainShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, m_diffuse );
        shaderLoc = glGetUniformLocation( mainShader, "Shininess" );
        glUniform1f( shaderLoc, m_shininess );

        glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() );

     }

     // ----- Lighted Plane -----
     
    LightedPlane::LightedPlane() : LightedRecursiveSphere( 0,0,0,0) {
       m_vertices.clear();
       m_normals.clear();
       generate();
    }

    void LightedPlane::generate() {
        float m_sideLength = 1000;
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); 

        m_vertices.push_back( vec4( -m_sideLength / 2, 0, -m_sideLength / 2, 1 ) );

        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) );

        m_vertices.push_back( vec4( m_sideLength / 2, 0, m_sideLength / 2, 1 ) );

        vec3 E = daveutils::FourDto3d(m_vertices[0]);
        vec3 F = daveutils::FourDto3d(m_vertices[1]);
        vec3 G = daveutils::FourDto3d(m_vertices[2]);
        vec3 normal = -normalize( cross( F - E, G - E )); 
        m_normals.push_back( normal );
        m_normals.push_back( normal );
        m_normals.push_back( normal );
        m_normals.push_back( normal );


    }


    void LightedPlane::draw(std::map<std::string, GLuint>& shaderMap) {
        GLuint mainShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];

        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot);

        glUseProgram( mainShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( mainShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        // Set material property uniforms
        GLuint shaderLoc = glGetUniformLocation( mainShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, m_ambient );
        shaderLoc = glGetUniformLocation( mainShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, m_specular );
        shaderLoc = glGetUniformLocation( mainShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, m_diffuse );
        shaderLoc = glGetUniformLocation( mainShader, "Shininess" );
        glUniform1f( shaderLoc, m_shininess );

        glDrawArrays( GL_TRIANGLE_FAN, 0, m_vertices.size() );

    }

}
