#ifndef __DICE_ROLLER_SCENE_H__
#define __DICE_ROLLER_SCENE_H__
#include "scene.hpp"
namespace dgfx {
class DiceRollerScene : public Scene {
    public:
        DiceRollerScene();
    const static std::string FRAGMENT_LIGHTING_SHADER_NAME,
          VERTEX_LIGHTING_SHADER_NAME, FRAGMENT_TEXTURE_SHADER_NAME;

    protected:
	 void keyboardHandler(unsigned char key, int x, int y);
         void specialKeyHandler(int key, int x, int y);
	 void clickHandler(GLint button, GLint state, GLint x, GLint y);
	 void displayCallback();
	 void timerCallback( int value );
};
}
#endif
