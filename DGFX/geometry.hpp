#ifndef __DGFX_GEOMETRY_H__
#define __DGFX_GEOMETRY_H__
#include "mat.h"
#include "entity.hpp"
#include "Angel.h"
namespace dgfx {
    typedef vec4 point4;

    // 2D stuff
	
    class Polygon : public Entity{
    public:
        Polygon( std::vector<vec4> vertices, std::vector<vec4> colors, float x, float y );
        Polygon( std::vector<vec4> vertices, float x, float y); 

    protected:
        std::vector<vec4> m_vertices;
        std::vector<vec4> m_vertexColors;
        mat4 m_modelMatrix;
        float m_x, m_y;
        float m_theta = 0;
        bool m_specialMode = false;

        virtual mat4 calculateModelMatrix ( float theta );


        // Update the GL data
        virtual void init(std::map<std::string, GLuint>& shaderMap) = 0;
        virtual void update(std::map<std::string, GLuint>& shaderMap) = 0;
        virtual void draw(std::map<std::string, GLuint>& shaderMap) = 0;

        // Called by the scene on an event
        virtual void keyboardHandler(unsigned char key, int x, int y);
        virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);
        virtual void specialKeyHandler(int key, int x, int y);


    private:
        void updateBuffers();
    };

    class SingleColorPolygon : public Polygon {
    public:
        SingleColorPolygon(std::vector<vec4> vertices, vec4 color, float x, float y);

    protected: 
        const std::string SINGLE_COLOR_SHADER_NAME = "a2_single";

        vec4 m_color, m_currentColor;

        void init(std::map<std::string, GLuint>& shaderMap);
        void update(std::map<std::string, GLuint>& shaderMap);
        void draw(std::map<std::string, GLuint>& shaderMap);

    };

    class MulticolorPolygon : public Polygon {
    public:
        MulticolorPolygon(std::vector<vec4> vertices, std::vector<vec4> colors, float x, float y);

    protected:
        const std::string MULTICOLOR_SHADER_NAME = "a2_multi";
        void init(std::map<std::string, GLuint>& shaderMap);
        void update(std::map<std::string, GLuint>& shaderMap);
        void draw(std::map<std::string, GLuint>& shaderMap);
    };

    // -------------------------------------
    // 3D 
    // ------------------------------------
    //
    
    class Model : public Entity {

        public:
            Model( float x,
                   float y,
                   float z );

            Model( float x,
                     float y, 
                     float z,
                     uint16_t n,
                     float size, 
                     float depth);

            void translate (float x, float y, float z);
            void rotate    (float x, float y, float z);

        private:
        float m_size, m_depth;

        protected:
        uint16_t m_n;
        std::vector<vec4> m_vertices, m_colors;
        std::vector<vec3> m_normals;
        std::vector<GLuint> m_elements;
        float m_x, m_y, m_z, m_xRot, m_yRot, m_zRot;
        vec4 m_frameColor;
        virtual void generate();

        void calculateNormals();


        // Called by the scene to set up GL data structures
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        
        // Called by the scene to update GL state based on internal state
        virtual void update(std::map<std::string, GLuint>& shaderMap);

        // Called by the scene to draw the object
        virtual void draw(std::map<std::string, GLuint>& shaderMap);

        // Called by the scene on an event
        virtual void keyboardHandler(unsigned char key, int x, int y);
        virtual void clickHandler(GLint button, GLint state, GLint x, GLint y);
        virtual void specialKeyHandler(int key, int x, int y);
    };

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

    class FloorPlane : public Model {
    public:
        FloorPlane( uint16_t sideLength, vec4 color );
    private:
        uint16_t m_sideLength;
        vec4 m_color;
    protected:
        void generate();


        // Called by the scene to draw the object
        void draw(std::map<std::string, GLuint>& shaderMap);

    };

    class LightedRecursiveSphere : public RecursiveSphere {
    public:
    LightedRecursiveSphere( float x,
                     float y,
                     float z,
                     uint16_t numRecursions );

    protected:
        vec4 m_ambient, m_diffuse, m_specular;
        float m_shininess;
        // Called by the scene to draw the object
        virtual void init(std::map<std::string, GLuint>& shaderMap);
        virtual void draw(std::map<std::string, GLuint>& shaderMap);

    };

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

    class LightedPlane : public LightedRecursiveSphere {
    public:
        LightedPlane();
    protected:
        void generate();
        void draw(std::map<std::string, GLuint>& shaderMap);
    };
    
}

#endif
