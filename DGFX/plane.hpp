#ifndef __PLANE_H__
#define __PLANE_H__
#include "object.hpp"
namespace dgfx {
class Plane : public Object {
public:
    Plane ( float x, float y, float z, float xrot,float yrot, float zrot , float sideLength);

protected:
    float m_sideLength;
    virtual void generateGeometry();
    virtual void textureInit();
    virtual void textureDraw();
    virtual void setShader( std::map<std::string, GLuint>& shaderMap );
};
}
#endif
