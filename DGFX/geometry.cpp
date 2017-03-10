#include <iostream>
#include <string>

#include "geometry.hpp"
#include "daveutils.hpp"
#include "scene.hpp"

namespace dgfx {


    // -------------------------------------
    // 3D 
    // ------------------------------------
    //
    //

    // ----- RecursiveSphere -----
    
    /*
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
*/

    /*
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

    */


    /* ----- LightedGeometry ----- */
    
    LightedGeometry::LightedGeometry(float x, float y, float z) :
        m_x(x),
        m_y(y),
        m_z(z){
            
            m_ambient = vec4( 1.0, 1.0, 0.0, 1.0 );
            m_diffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
            m_specular = vec4( 1.0, 1.0, 0.0, 1.0 );
            m_shininess = 5.0;
    }

    // Copies any texture coords to GPU and sets up the attrib pointer
    void LightedGeometry::textureInit(){}
    void LightedGeometry::textureDraw(){}


   
    unsigned char * LightedGeometry::ppmRead(char* filename, int* width, int* height) {
	FILE* fp=NULL;
	int i, w, h, d;
	unsigned char* image;
	char head[70];		// max line <= 70 in PPM (per spec).
#ifdef WIN32
	fopen_s(&fp, filename, "rb");
#else
	fp = fopen(filename, "rb");
#endif
	if (fp==NULL) {
	    perror(filename);
	    return NULL;
	}
	
	// Grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	fgets(head, 70, fp);
	if (strncmp(head, "P6", 2)) {
	    fprintf(stderr, "%s: Not a raw PPM file\n", filename);
	    return NULL;
	}
	
	// Grab the three elements in the header (width, height, maxval).
	i = 0;
	while (i < 3) {
	    fgets(head, 70, fp);
	    if (head[0] == '#')		// skip comments.
		continue;
	    if (i == 0){
#ifdef WIN32
		i += sscanf_s(head, "%d %d %d", &w, &h, &d);
#else
		i += sscanf(head, "%d %d %d", &w, &h, &d);
#endif
	    }
	    else if (i == 1){
#ifdef WIN32
		i += sscanf_s(head, "%d %d", &h, &d);
#else
		i += sscanf(head, "%d %d", &h, &d);
#endif
	    }
	    else if (i == 2){
#ifdef WIN32
		i += sscanf_s(head, "%d", &d);
#else
		i += sscanf(head, "%d", &d);
#endif
	    }
	}
	
	// Grab all the image data in one fell swoop.
	image = (unsigned char*)malloc(sizeof(unsigned char) * w * h * 3);
	fread(image, sizeof(unsigned char), w * h * 3, fp);
	fclose(fp);
	
	*width = w;
	*height = h;
	return image;


    }


    // Called by the scene to draw the object
    void LightedGeometry::init(std::map<std::string, GLuint>& shaderMap) {
        generateGeometry();
        calculateNormals();

        std::cout << "Init with active shader = " << m_activeShader << std::endl;


        m_vertexBuffers.resize(3);
        m_vertexArrays.resize(1);

        glGenBuffers(3, &m_vertexBuffers[0]);
        glGenVertexArrays( 1, &m_vertexArrays[0]);

        glBindVertexArray( m_vertexArrays[0] );

        // Copy vertex position data and set up the attribute pointers
        
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[0] );
        glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec4), &m_vertices[0], GL_STATIC_DRAW );
        GLuint vPositionLoc = glGetAttribLocation( m_activeShader, "vPosition" );
        glEnableVertexAttribArray( vPositionLoc );
        glVertexAttribPointer( vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Copy the normals
        glBindBuffer( GL_ARRAY_BUFFER , m_vertexBuffers[1] );
        glBufferData( GL_ARRAY_BUFFER, m_normals.size() * sizeof(vec3), &m_normals[0], GL_STATIC_DRAW );
        GLuint vNormalLoc = glGetAttribLocation( m_activeShader, "vNormal" );
        glEnableVertexAttribArray( vNormalLoc );
        glVertexAttribPointer( vNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

        // Do texture-related initialization
        textureInit();

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
    void LightedGeometry::draw(std::map<std::string, GLuint>& shaderMap) {
        
        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateX( m_xRot ) * RotateY(m_yRot) * RotateZ( m_zRot);

        glUseProgram( m_activeShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( m_activeShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        // Set material property uniforms
        GLuint shaderLoc = glGetUniformLocation( m_activeShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, m_ambient );
        shaderLoc = glGetUniformLocation( m_activeShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, m_specular );
        shaderLoc = glGetUniformLocation( m_activeShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, m_diffuse );
        shaderLoc = glGetUniformLocation( m_activeShader, "Shininess" );
        glUniform1f( shaderLoc, m_shininess );

        glUniform1i( glGetUniformLocation( m_activeShader, "EnableLighting" ), 1 );

        // Texture-related draw commands
        textureDraw();

        glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() );

    }
    // Called by the scene to update GL state based on internal state
    void LightedGeometry::update(std::map<std::string, GLuint>& shaderMap) { 
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

    void LightedGeometry::calculateNormals() {
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

    // Called by the scene on an event
    void LightedGeometry::keyboardHandler(unsigned char key, int x, int y) {}

    void LightedGeometry::clickHandler(GLint button, GLint state, GLint x, GLint y) {}

    void LightedGeometry::specialKeyHandler(int key, int x, int y){}

    /* --- LightedCube --- */

    LightedCube::LightedCube(float x, float y, float z) : LightedGeometry(x,y,z) {}

    void LightedCube::generateGeometry() {
  	vec4 vertices[] = {vec4(-0.5,-0.5,0.5,1.0),vec4(-0.5,0.5,0.5,1.0),vec4(0.5,0.5,0.5,1.0),vec4(0.5,-0.5,0.5,1.0), vec4(-0.5,-0.5,-0.5,1.0),vec4(-0.5,0.5,-0.5,1.0),vec4(0.5,0.5,-0.5,1.0),vec4(0.5,-0.5,-0.5,1.0)};

         makeQuad(1,0,3,2, vertices);  //front
         makeQuad(2,3,7,6, vertices);  //right
         makeQuad(3,0,4,7, vertices);  //bottom
         makeQuad(6,5,1,2, vertices);  //top
         makeQuad(4,5,6,7, vertices);  //back
         makeQuad(5,4,0,1, vertices);  //left
    }

    void LightedCube::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];
    }

    void LightedCube::makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices){
      //Triangle #1
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[b]);
      m_vertices.push_back(vertices[c]);

      //Triangle #2
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[c]);
      m_vertices.push_back(vertices[d]);

    }


    LightedRecursiveSphere::LightedRecursiveSphere( float x,
                         float y,
                         float z,
                         uint16_t numRecursions ) : 
                         LightedGeometry( x, y, z ){
        m_recursionDepth = numRecursions;
    }


    void LightedRecursiveSphere::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];
    }

    point4 LightedRecursiveSphere::unit( const point4 &p ) {
        float len = p.x*p.x + p.y*p.y + p.z*p.z;
        
        point4 t;
        if ( len > DivideByZeroTolerance ) {
            t = p / sqrt(len);
            t.w = 1.0;
        }

        return t;

    }

    void LightedRecursiveSphere::triangle( const point4& a, 
                       const point4& b, 
                       const point4& c ) {

        m_vertices.push_back( a );
        m_vertices.push_back( b );
        m_vertices.push_back( c );
    }

    void LightedRecursiveSphere::tetrahedron( int count ) {
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

    void LightedRecursiveSphere::divide_triangle( const point4 &a, 
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

    void LightedRecursiveSphere::generateGeometry() {
        tetrahedron( m_recursionDepth );
    }


    LightedPlane::LightedPlane() : LightedGeometry( 0,0,0) {
    }

    void LightedPlane::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ A5Scene::FRAGMENT_LIGHTING_SHADER_NAME ];
    }

    void LightedPlane::generateGeometry() {
        float m_sideLength = 50;
        /*
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); // TL

        m_vertices.push_back( vec4( -m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BL

        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BR

        m_vertices.push_back( vec4( m_sideLength / 2, 0, m_sideLength / 2, 1 ) ); // TR
        */
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); // TL
        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BR
        m_vertices.push_back( vec4( -m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BL

        m_vertices.push_back( vec4( m_sideLength / 2, 0, m_sideLength / 2, 1 ) ); // TR
        m_vertices.push_back( vec4( m_sideLength / 2, 0, -m_sideLength / 2, 1 ) ); // BR
        m_vertices.push_back( vec4( -m_sideLength / 2, 0,  m_sideLength / 2, 1 ) ); // TL
    }

    TexturedLightedCube::TexturedLightedCube(float x, float y, float z) : LightedCube(x,y,z){}

    void TexturedLightedCube::generateGeometry() {
        LightedCube::generateGeometry();

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

    void TexturedLightedCube::textureInit() {
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

    void TexturedLightedCube::textureDraw() {

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


    void TexturedLightedCube::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ A5Scene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }

    void TexturedLightedCube::keyboardHandler(unsigned char key, int x, int y) {
        if (key == 't' )
            m_activeTextureHandle = !m_activeTextureHandle;

    }

    // -----------------------
    TexturedLightedPlane::TexturedLightedPlane( float x, float y, float z, float yrot, float zrot) :
        LightedPlane() {
            m_x = x;
            m_y = y;
            m_z = z;
            m_yRot = yrot;
            m_zRot = zrot;
        }


    void TexturedLightedPlane::generateGeometry() {
        LightedPlane::generateGeometry();

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

    void TexturedLightedPlane::textureInit() {
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

    void TexturedLightedPlane::textureDraw() {

        glEnable( GL_TEXTURE_2D );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );

        glUniform1i( glGetAttribLocation( m_activeShader, "textureID" ), 0 );

    }


    void TexturedLightedPlane::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ A5Scene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }

    void TexturedLightedPlane::keyboardHandler(unsigned char key, int x, int y) {
    }
}

