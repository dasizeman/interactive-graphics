#ifndef __DGFX_SCENE_H__
#define __DGFX_SCENE_H__

#include <string>
#include <map>
#include <memory>

#include "entity.h"

namespace dgfx {
    class Scene {
	private:
	    std::map<std::string, GLuint> m_shaderMap;
	    std::vector<std::unique_ptr<Entity>> m_entities;

	    void init();
	    void init(std::string windowName, int width, int height);
	    void addShader ( std::string shaderName );

	    void keyboardHandler(unsigned char key, int x, int y);
	    void clickHandler(GLint button, GLint state, GLint x, GLint y);

	    void displayCallback();
	    void timerCallback( int value );

	public:
	    Scene();
	    Scene(std::string windowName, int width, int height);

	    void addEntity(std::unique_ptr<Entity> entity);
	    void start();
    };
}
#endif
