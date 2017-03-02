#include "Angel.h"
#include "light.hpp"

namespace dgfx {

    Light::Light( LightType type,
                  vec4 position,
                  vec4 ambient,
                  vec4 diffuse,
                  vec4 specular ) :
        m_type( type ),
        m_position( position ),
        m_ambient( ambient ),
        m_diffuse( diffuse ),
        m_specular( specular ){}
}
