#ifndef __DAVE_UTILS_H_
#define __DAVE_UTILS_H_
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

#include "Angel.h"

namespace daveutils {
    float randomFloat(float min, float max);

    std::vector<vec4> generateSquareVertices( uint16_t centerX, uint16_t centerY, uint16_t edgeLength );

    std::vector<vec4> generateTriangleVertices( uint16_t centerX, uint16_t centerY, uint16_t size );

    void convertToCameraCoordinates( std::vector<vec4> &vertices, uint16_t screenWidth, uint16_t screenHeight );

    void convertClickCoordinates( float &x, float &y, int screenWidth, int screenHeight);

    template <typename T>
    std::string printVector ( std::vector<T> &vec) {
        std::stringstream ss;
        ss << "{ ";
       for ( T item : vec )
            ss << item << ", ";

       ss << "}";
       return ss.str();
    }
}
#endif
