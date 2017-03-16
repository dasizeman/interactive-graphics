#ifndef __CUBE_H__
#define __CUBE_H__
#include "object.hpp"
namespace dgfx {
class Cube : public Object {
    public:
        Cube(float x, float y, float z, float xrot, float yrot, float zrot);
        GLuint m_activeTextureHandle = 0;
    protected:
        void makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices);
        virtual void generateGeometry();
        virtual void textureInit();
        virtual void textureDraw();
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );
        virtual void keyboardHandler(unsigned char key, int x, int y);
};
}
#endif
