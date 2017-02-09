#include <ctime>
#include <cstdlib>
#include <iostream>

#include "daveutils.hpp"

static bool randSeeded = false;
namespace daveutils {
    float randomFloat(float min, float max) {
	if (!randSeeded) {
	    srand(unsigned(time(0)));
	    randSeeded = true;
	}
	return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
	
    }


    std::vector<vec4> generateSquareVertices( uint16_t centerX, uint16_t centerY, uint16_t edgeLength ) {
        uint16_t offset = edgeLength / 2;
        std::vector<vec4> result = { 
            vec4( centerX - offset, centerY - offset, 0 ,1 ),
            vec4( centerX - offset, centerY + offset, 0, 1 ),
            vec4( centerX + offset, centerY + offset, 0, 1),
            vec4( centerX + offset, centerY - offset, 0, 1)
        };

        return result;
    }

    std::vector<vec4> generateTriangleVertices( uint16_t centerX, uint16_t centerY, uint16_t sideLength ) {
       float xDist = static_cast<float>(sideLength) / 2;
       float yDist = static_cast<float>( sqrt(3) / 4) * sideLength;
       std::vector<vec4> result = {
           vec4( centerX, static_cast<int16_t>(centerY - yDist), 0, 1 ),
           vec4( static_cast<int16_t>(centerX - xDist), static_cast<int16_t>(centerY + yDist),0, 1 ),
           vec4( static_cast<int16_t>(centerX + xDist), static_cast<int16_t>(centerY + yDist),0, 1 )
       };

       return result;

    }


    void convertToCameraCoordinates( std::vector<vec4> &vertices, uint16_t screenWidth, uint16_t screenHeight ) {
        for ( auto it = vertices.begin(); it != vertices.end(); it++ ) {
            vec4 transformed;
            transformed.x = 2 * (static_cast<float>(it->x) / screenWidth) - 1;
            transformed.y = 2 * (static_cast<float>(screenHeight - it->y) / screenHeight) - 1;
            transformed.z = 0;
            transformed.w = 1;

            *it = transformed;
        }
    }

    void convertClickCoordinates( float &x, float &y, int screenWidth, int screenHeight) {
        float newX = 2 * (static_cast<float>(x) / static_cast<float>(screenWidth)) - 1;
        float newY = 2 * (static_cast<float>(screenHeight - y) / static_cast<float>(screenHeight)) - 1;

        x = newX;
        y = newY;
    }



}

