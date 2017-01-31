#include "2d.h"

namespace dgfx {

	/* Polygon */
	
	// VBO Map:
	// 0 - Vertices
	void Polygon::_init(std::vector<GLuint> &vertexBuffers,
			  std::vector<GLuint> &vertexArrays,
			  GLuint &shader) {
	    //TODO compile shader and setup VAOS/VBOS, and uniforms
	}

	void Polygon::_update(std::vector<GLuint> &vertexBuffers,
			   std::vector<GLuint> &vertexArrays,
			   GLuint &shader) {
	    // Copy vertex data to the VBO

	}

	// Note that you can not draw a Polygon, you must draw a subclass
	
	void Polygon::setVelocity(vec3 velocity) {
	    // TODO set the translation matrix
	    update();
	    
	}

	void Polygon::setRotationAngle(float theta) {
	    // TODO set the rotation matrix
	    update();
	}


	// Note that you can not draw a Polygon, you must draw a subclass
	
	/* Uniform Polygon */
	void UniformPolygon::_init(std::vector<GLuint> &vertexBuffers,
			  std::vector<GLuint> &vertexArrays,
			  GLuint &shader) {
	    Polygon::_init(vertexBuffers, vertexArrays, shader);
	    //TODO Set the shader and the color uniform value
	}

	void UniformPolygon::_draw(std::vector<GLuint> &vertexBuffers,
			   std::vector<GLuint> &vertexArrays,
			   GLuint &shader) {
	}

	void UniformPolygon::_update(std::vector<GLuint> &vertexBuffers,
			   std::vector<GLuint> &vertexArrays,
			   GLuint &shader) {
	    Polygon::_update( vertexBuffers, vertexArrays, shader );
	    // TODO set the color uniform

	}

}
