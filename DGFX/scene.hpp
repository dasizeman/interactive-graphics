#ifndef __DGFX_SCENE_H__
#define __DGFX_SCENE_H__

#include <string>
#include <map>
#include <memory>
#include <cstdint>

#include "Angel.h"
#include "entity.hpp"

namespace dgfx {
    class Scene {
    public:
        static std::shared_ptr<Scene> m_instance;
    protected:
        std::map<std::string, GLuint> m_shaderMap;
        std::vector<std::unique_ptr<Entity>> m_entities;
        bool m_doAnimation = false;

        uint16_t m_screenWidth, m_screenHeight;

        const static std::string DEFAULT_WINDOW_NAME;
        const static std::string SHADER_PATH;
        const static int DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT;

        void init(std::string windowName, int width, int height);
        void addShader ( std::string shaderName );

	virtual void keyboardHandler(unsigned char key, int x, int y);
        virtual void specialKeyHandler(int key, int x, int y);

	virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);

	virtual void displayCallback();
	virtual void timerCallback( int value );

        void glewInitAndVersion(void);


    public:
        Scene();
        Scene(std::string windowName, int width, int height);


        void addEntity(std::unique_ptr<Entity> entity);
        void start();
        void globalAnimationToggle();

        static void keyboard_callback_wrapper(unsigned char key, int x, int y);
        static void special_key_wrapper(int key, int x, int y);
        static void click_callback_wrapper(GLint button, GLint state, GLint x, GLint y);
        static void display_callback_wrapper();
        static void timer_callback_wrapper( int value );
        static void close_handler();
    };

    class A2Scene : public Scene {
	 void keyboardHandler(unsigned char key, int x, int y);
	 void clickHandler(GLint button, GLint state, GLint x, GLint y);
    };

}
#endif
