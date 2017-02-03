#ifndef __DGFX_SCENE_H__
#define __DGFX_SCENE_H__

#include <string>
#include <map>
#include <memory>

#include "Angel.h"
#include "entity.h"

namespace dgfx {
    class Scene {
	private:
	    std::map<std::string, GLuint> m_shaderMap;
	    std::vector<std::unique_ptr<Entity>> m_entities;

	    const std::string DEFAULT_WINDOW_NAME = "CS432";
	    const std::string SHADER_PATH = "shaders/";
	    const int DEFAULT_WINDOW_WIDTH = 500, DEFAULT_WINDOW_HEIGHT = 500;

	    void init();
	    void init(std::string windowName, int width, int height);
	    void addShader ( std::string shaderName );

	    void keyboardHandler(unsigned char key, int x, int y);
	    void clickHandler(GLint button, GLint state, GLint x, GLint y);

	    void displayCallback();
	    void timerCallback( int value );

	    void glewInitAndVersion(void);
        void cleanup();

	    static void display_callback_wrapper();
	    static void timer_callback_wrapper( int value );

	public:
	    Scene();
	    Scene(std::string windowName, int width, int height);


	    void addEntity(std::unique_ptr<Entity> entity);
	    void start();

	    static Scene& getInstance();

	    static void keyboard_callback_wrapper(unsigned char key, int x, int y);
	    static void click_callback_wrapper(GLint button, GLint state, GLint x, GLint y);
        static void close_handler();
    };
}
#endif
