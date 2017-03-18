#include "object.hpp"
namespace dgfx {

    Object::Object(float x, float y, float z, float xrot, float yrot, float zrot) :
        m_x(x),
        m_y(y),
        m_z(z),
        m_xRot(xrot),
        m_yRot(yrot),
        m_zRot(zrot){
            m_xRot = 0;
            m_yRot = 0;
            m_zRot = 0;
            
            m_ambient = vec4( 1.0, 1.0, 0.0, 1.0 );
            m_diffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
            m_specular = vec4( 1.0, 1.0, 0.0, 1.0 );
            m_shininess = 5.0;
    }

   
    unsigned char * Object::ppmRead(char* filename, int* width, int* height) {
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
    void Object::init(std::map<std::string, GLuint>& shaderMap) {
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
    void Object::draw(std::map<std::string, GLuint>& shaderMap) {
        
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
    void Object::update(std::map<std::string, GLuint>& shaderMap) { 
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

    void Object::calculateNormals() {
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
    void Object::keyboardHandler(unsigned char key, int x, int y) {}

    void Object::clickHandler(GLint button, GLint state, GLint x, GLint y) {}

    void Object::specialKeyHandler(int key, int x, int y){}

    // Object picking
    void Object::wasPicked( uint16_t triangleIdx ){}


}
