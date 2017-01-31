#ifndef __DGFX_DRAWABLE_H__
#define __DGFX_DRAWABLE_H__
#include "Angel.h"
#include <vector>

namespace dgfx {
class Drawable {
    private:
	GLuint m_shader;
	std::vector<GLuint> m_vertexBuffers, m_vertexArrays;

    protected:
	virtual void _init(std::vector<GLuint> &vertexBuffers,
			  std::vector<GLuint> &vertexArrays,
			  GLuint &shader) = 0;
	virtual void _draw(std::vector<GLuint> &vertexBuffers,
			   std::vector<GLuint> &vertexArrays,
			   GLuint &shader) = 0;
	virtual void _update(std::vector<GLuint> &vertexBuffers,
			   std::vector<GLuint> &vertexArrays,
			   GLuint &shader) = 0;

	// Update the GL data
	void update() { _update(m_vertexBuffers, m_vertexArrays, m_shader); }


    public:
	Drawable(){ _init(m_vertexBuffers, m_vertexArrays, m_shader);}
	~Drawable() { glDeleteBuffers ( m_vertexBuffers.size(), &m_vertexBuffers[0] );}
	void draw() { _draw(m_vertexBuffers, m_vertexArrays, m_shader); };
};
}

#endif
