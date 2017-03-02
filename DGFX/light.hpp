#ifndef __DGFX_LIGHT_H__
#define __DGFX_LIGHT_H__
namespace dgfx {
    typedef enum {
        DIRECTIONAL,
        SPOTLIGHT
    } LightType;

    class Light {
        public:
            Light( LightType type,
                    vec4 position,
                    vec4 ambient,
                    vec4 diffuse,
                    vec4 specular );
            vec4 m_position;
            LightType m_type;
            vec4 m_ambient, m_diffuse, m_specular;
    };
}

#endif
