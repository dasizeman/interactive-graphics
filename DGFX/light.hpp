#ifndef __DGFX_LIGHT_H__
#define __DGFX_LIGHT_H__
#include "Angel.h"
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
            vec4 m_direction;
            LightType m_type;
            vec4 m_ambient, m_diffuse, m_specular;
            int m_toggle;
    };
}

#endif
