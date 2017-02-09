#ifndef __DGFX_2D_H__
#define __DGFX_2D_H__
#include "mat.h"
#include "entity.hpp"
namespace dgfx {
	
    class Polygon : public Entity{
    public:
        Polygon( std::vector<vec3> vertices, std::vector<vec4> colors, float x, float y );
        Polygon( std::vector<vec3> vertices, float x, float y); 

    protected:
        std::vector<vec3> m_vertices;
        std::vector<vec4> m_vertexColors;
        mat3 m_modelMatrix;
        float m_x, m_y;
        float m_theta = 0;
        bool m_specialMode = false;

        virtual mat3 calculateModelMatrix ( float theta );


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
        SingleColorPolygon(std::vector<vec3> vertices, vec4 color, float x, float y);

    protected: 
        const std::string SINGLE_COLOR_SHADER_NAME = "a2_single";

        vec4 m_color, m_currentColor;

        void init(std::map<std::string, GLuint>& shaderMap);
        void update(std::map<std::string, GLuint>& shaderMap);
        void draw(std::map<std::string, GLuint>& shaderMap);

    };

    class MulticolorPolygon : public Polygon {
    public:
        MulticolorPolygon(std::vector<vec3> vertices, std::vector<vec4> colors, float x, float y);

    protected:
        const std::string MULTICOLOR_SHADER_NAME = "a2_multi";
        void init(std::map<std::string, GLuint>& shaderMap);
        void update(std::map<std::string, GLuint>& shaderMap);
        void draw(std::map<std::string, GLuint>& shaderMap);
    };
}

#endif
