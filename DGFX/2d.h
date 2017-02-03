#ifndef __DGFX_2D_H__
#define __DGFX_2D_H__
#include "mat.h"
#include "entity.h"
namespace dgfx {
	
    class Polygon : public Entity{
	public:
	    Polygon(std::vector<vec3> vertices, std::vector<vec4> colors);
	    ~Polygon();

	protected:
	    mat4 m_rotationMatrix, m_translationMatrix;
	    std::vector<vec3> m_vertices;
	    std::vector<vec4> m_vertexColors;

	    // Update the GL data
	    virtual void init(std::map<std::string, GLuint>& shaderMap);
	    virtual void update(std::map<std::string, GLuint>& shaderMap);
	    void draw(std::map<std::string, GLuint>& shaderMap);

	    // Called by the scene on an event
	    virtual void keyboardHandler(unsigned char key, int x, int y);
	    virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);

	private:
	    void updateBuffers();
    };
}

#endif
