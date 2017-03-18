#ifndef __DICE_ROLLER_SCENE_H__
#define __DICE_ROLLER_SCENE_H__
#include "scene.hpp"
namespace dgfx {
class DiceRollerScene : public Scene {
    public:
        DiceRollerScene(std::string windowName, int width, int height);
    const static std::string FRAGMENT_LIGHTING_SHADER_NAME,
          VERTEX_LIGHTING_SHADER_NAME, FRAGMENT_TEXTURE_SHADER_NAME;

    protected:
         // Handlers/ callbacks
	 void keyboardHandler(unsigned char key, int x, int y);
         void specialKeyHandler(int key, int x, int y);
	 void clickHandler(GLint button, GLint state, GLint x, GLint y);
	 void displayCallback();
	 void timerCallback( int value );
         void reshapeCallback( int width, int height );

         // Update the aspect ratios on all cameras
         void updateCameraAspectRatios(int width, int height);

         // Obeject picking stuff
         void pickTriangle( uint16_t x, uint16_t y );
         float rayIntersectsPlane( vec3 ray, vec3 e, vec3 f, vec3 g );
         bool insideTest( vec3 e, vec3 f, vec3 g, vec3 point );

};
}
#endif
