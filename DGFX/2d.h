#ifndef __DGFX_2D_H__
#define __DGFX_2D_H__
#include "drawable.h"
#include "mat.h"
namespace dgfx {
	
    class Polygon : public Drawable {
	public:
	    Polygon(std::vector<vec2> vertices) : Drawable(),
		m_vertices(vertices){}

	protected:
	    mat4 m_rotationMatrix, m_translationMatrix;
	    std::vector<vec2> m_vertices;

	
	    virtual void _init(std::vector<GLuint> &vertexBuffers,
			      std::vector<GLuint> &vertexArrays,
			      GLuint &shader);

	    virtual void _update(std::vector<GLuint> &vertexBuffers,
			       std::vector<GLuint> &vertexArrays,
			       GLuint &shader);

	    // Note that you can not draw a Polygon, you must draw a subclass
	
	public:
	    void setVelocity(vec3 velocity);
		

	    void setRotationAngle(float theta);
    };

    class UniformPolygon : public Polygon {
	public:
	    UniformPolygon(std::vector<vec2> vertices, vec4 color) : Polygon( vertices ),
		m_color(color){}

	protected:
	    vec4 m_color;

	    void _init(std::vector<GLuint> &vertexBuffers,
			      std::vector<GLuint> &vertexArrays,
			      GLuint &shader);

	    void _draw(std::vector<GLuint> &vertexBuffers,
			       std::vector<GLuint> &vertexArrays,
			       GLuint &shader);

	    void _update(std::vector<GLuint> &vertexBuffers,
			       std::vector<GLuint> &vertexArrays,
			       GLuint &shader);
    };
}

#endif
