#ifndef __DGFX_DRAWABLE_H__
#define __DGFX_DRAWABLE_H__
#include <vector>
#include <string>
#include <map>

#include "Angel.h"

namespace dgfx {
class Entity {
    friend class Scene;

    public:
    virtual ~Entity();

    protected:
	std::vector<GLuint> m_vertexBuffers, m_vertexArrays;
	std::vector<std::string>  m_shaderNames;
	
	// Called by the scene to set up GL data structures
	virtual void init(std::map<std::string, GLuint>& shaderMap) = 0;
	
	// Called by the scene to update GL state based on internal state
	virtual void update(std::map<std::string, GLuint>& shaderMap) = 0;

	// Called by the scene to draw the object
	virtual void draw(std::map<std::string, GLuint>& shaderMap) = 0;

	// Called by the scene on an event
	virtual void keyboardHandler(unsigned char key, int x, int y) = 0;
	virtual void clickHandler(GLint button, GLint state, GLint x, GLint y)=0;
	
	// Called by the scene to determine which shaders to load
	const std::vector<std::string> &getShaderNames();
	

};
}

#endif
