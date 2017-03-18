#ifndef __OBJECT_H__
#define  __OBJECT_H__
#include "entity.hpp"
#include "scene.hpp"
namespace dgfx {
class Object : public Entity {
    friend class DiceRollerScene;
    public:
        Object(float x, float y, float z, float xrot, float yrot, float zrot);
    protected:


        float m_x, m_y, m_z;
        vec4 m_ambient, m_diffuse, m_specular;
        std::vector<vec4> m_vertices;
        std::vector<vec3> m_normals;
        std::vector<vec2> m_textureCoords;
        std::vector<GLuint> m_textureHandles;
        float m_shininess;
        float m_xRot ,m_yRot, m_zRot;

        GLuint m_activeShader;

        // Read PPM image into texture
        unsigned char * ppmRead(char* filename, int* width, int* height);


        // Called by the scene to draw the object
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        virtual void draw(std::map<std::string, GLuint>& shaderMap);
        // Called by the scene to update GL state based on internal state
        virtual void update(std::map<std::string, GLuint>& shaderMap);

        // Called by the scene on an event
        virtual void keyboardHandler(unsigned char key, int x, int y);
        virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);
        virtual void specialKeyHandler(int key, int x, int y);

        virtual void generateGeometry() = 0;
        void calculateNormals();

        // Populate the vertex and normal vectors
        virtual void textureInit() = 0;
        virtual void textureDraw() = 0;
        virtual void setShader( std::map<std::string, GLuint>& shaderMap ) = 0;

        // Called if a click of this object was detected using picking
        // The argument is the index into m_vertices of the face that was
        // clicked on
        virtual void wasPicked( uint16_t triangleIdx );
};
}
#endif
