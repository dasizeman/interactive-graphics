#ifndef __DGFX_GEOMETRY_H__
#define __DGFX_GEOMETRY_H__
#include "mat.h"
#include "entity.hpp"
#include "Angel.h"
namespace dgfx {
    typedef vec4 point4;


    /*
    class RecursiveSphere : public Model {
    friend class A4Scene;
    public:
        RecursiveSphere( float x,
                         float y,
                         float z,
                         uint16_t numRecursions );
        void blackenTriangle( uint16_t triangleIdx );

    private:
        uint16_t m_recursionDepth;

        point4 unit( const point4 &p );

        void triangle( const point4& a, 
                       const point4& b, 
                       const point4& c );

        void tetrahedron( int count );

        void divide_triangle( const point4 &a, 
                              const point4 &b,
                              const point4 &c,
                              int count );

    protected:
        virtual void generate();


        // Called by the scene to draw the object
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        virtual void draw(std::map<std::string, GLuint>& shaderMap);


    };
    */

    /*
    class LightedPolyhedron : public Model {
    public:

    LightedPolyhedron( float x,
             float y, 
             float z,
             uint16_t n,
             float size, 
             float depth);
    private:
        vec4 m_ambient, m_diffuse, m_specular;
        float m_shininess;

    protected:
        // Called by the scene to draw the object
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        virtual void draw(std::map<std::string, GLuint>& shaderMap);

    };
    */


    /* Base class for all geometry that responds to light */
    class LightedGeometry : public Entity {
    public:
        LightedGeometry(float x, float y, float z);
    protected:
        float m_x, m_y, m_z;
        vec4 m_ambient, m_diffuse, m_specular;
        std::vector<vec4> m_vertices;
        std::vector<vec3> m_normals;
        float m_shininess;
        float m_yRot;

        GLuint m_activeShader;


        // Called by the scene to draw the object
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        virtual void draw(std::map<std::string, GLuint>& shaderMap);
        // Called by the scene to update GL state based on internal state
        virtual void update(std::map<std::string, GLuint>& shaderMap);

        // Called by the scene on an event
        virtual void keyboardHandler(unsigned char key, int x, int y);
        virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);
        virtual void specialKeyHandler(int key, int x, int y);

        void calculateNormals();

        // Populate the vertex and normal vectors
        virtual void generateGeometry() = 0;
        virtual void setShader( std::map<std::string, GLuint>& shaderMap ) = 0;


    };

    class LightedRecursiveSphere : public LightedGeometry {
    public:
    LightedRecursiveSphere( float x,
                     float y,
                     float z,
                     uint16_t numRecursions );

    protected:
        // Populate the vertex and normal vectors
        virtual void generateGeometry();
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );

    private:
        uint16_t m_recursionDepth;

        point4 unit( const point4 &p );

        void triangle( const point4& a, 
                       const point4& b, 
                       const point4& c );

        void tetrahedron( int count );

        void divide_triangle( const point4 &a, 
                              const point4 &b,
                              const point4 &c,
                              int count );

    };

    class LightedPlane : public LightedGeometry {
    public:
        LightedPlane();
    protected:
        void generateGeometry();
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );
    };

    class LightedCube : public LightedGeometry {
    public:
        LightedCube(float x, float y, float z);
    protected:
        void generateGeometry();
        void makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices);
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );

    };
    
}

#endif
