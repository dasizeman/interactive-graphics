#ifndef __DGFX_SCENE_H__
#define __DGFX_SCENE_H__

#include <string>
#include <map>
#include <memory>
#include <cstdint>

#include "Angel.h"
#include "entity.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "entity.hpp"

#define FPS 100

namespace dgfx {
    class Entity;
    class Scene {
    public:
        static std::shared_ptr<Scene> m_instance;
    protected:
        std::vector<std::shared_ptr<Camera>> m_cameras;
        std::shared_ptr<Camera> m_activeCamera;
        std::map<std::string, GLuint> m_shaderMap;
        std::vector<std::unique_ptr<Entity>> m_entities;
        std::vector<Light> m_lights;
        bool m_doAnimation = false;

        uint16_t m_screenWidth, m_screenHeight;
        uint16_t m_miscIdx; // For testing

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
        virtual void reshapeCallback( int width, int height );

        void glewInitAndVersion(void);



    public:
        Scene();
        Scene(std::string windowName, int width, int height);


        void addEntity(std::unique_ptr<Entity> entity);
        void start();
        void globalAnimationToggle();
         // Remove dead things from the scene
         void removeDead();

        static void keyboard_callback_wrapper(unsigned char key, int x, int y);
        static void special_key_wrapper(int key, int x, int y);
        static void click_callback_wrapper(GLint button, GLint state, GLint x, GLint y);
        static void display_callback_wrapper();
        static void timer_callback_wrapper( int value );
        static void reshape_callback_wrapper( int width, int height );
        static void close_handler();

    };


    /*
    class A4Scene : public Scene {
    public:
        A4Scene();

    protected:
	 void keyboardHandler(unsigned char key, int x, int y);
         void specialKeyHandler(int key, int x, int y);
	 void displayCallback();
	 void clickHandler(GLint button, GLint state, GLint x, GLint y);
         void pickTriangle( uint16_t x, uint16_t y );
         float rayIntersectsPlane( vec3 ray, vec3 e, vec3 f, vec3 g );
         bool insideTest( vec3 e, vec3 f, vec3 g, vec3 point );
         vec3 FourDto3d( vec4 vector );
    };
    */

    class A5Scene : public Scene {
    public:
        A5Scene();
    const static std::string FRAGMENT_LIGHTING_SHADER_NAME,
          VERTEX_LIGHTING_SHADER_NAME, FRAGMENT_TEXTURE_SHADER_NAME;

    protected:
	 void keyboardHandler(unsigned char key, int x, int y);
         void specialKeyHandler(int key, int x, int y);
	 void displayCallback();
	 void timerCallback( int value );
    };



}
#endif
