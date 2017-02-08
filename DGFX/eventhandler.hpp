#ifndef __DGFX_EVENTHNANDLER_H__
#define __DGFX_EVENTHNANDLER_H__
namespace dgfx {
    class EventHandler {
        public:

        virtual void keyboardHandler(unsigned char key, int x, int y) = 0;
        virtual void specialKeyHandler(int key, int x, int y) = 0;

        virtual void clickHandler(GLint button, GLint state, GLint x, GLint y) = 0;

        virtual void displayHandler() = 0;
        virtual void timerHandler( int value ) = 0;

    };
}

#endif
