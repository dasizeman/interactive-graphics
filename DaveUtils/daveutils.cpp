#include <ctime>
#include <cstdlib>
#include <iostream>

#include "daveutils.h"

static bool randSeeded = false;
namespace daveutils {
    float randomFloat(float min, float max) {
	if (!randSeeded) {
	    srand(unsigned(time(0)));
	    randSeeded = true;
	}
	return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
	
    }


    std::vector<vec3> generateSquareVertices( uint16_t centerX, uint16_t centerY, uint16_t edgeLength ) {
        uint16_t offset = edgeLength / 2;
        std::vector<vec3> result = { 
            vec3( centerX - offset, centerY - offset, 1 ),
            vec3( centerX - offset, centerY + offset, 1 ),
            vec3( centerX + offset, centerY + offset, 1),
            vec3( centerX + offset, centerY - offset, 1)
        };

        return result;
    }

    std::vector<vec3> generateTriangleVertices( uint16_t centerX, uint16_t centerY, uint16_t sideLength ) {
       float xDist = static_cast<float>(sideLength) / 2;
       float yDist = static_cast<float>( sqrt(3) / 4) * sideLength;
       std::vector<vec3> result = {
           vec3( centerX, static_cast<int16_t>(centerY - yDist), 1 ),
           vec3( static_cast<int16_t>(centerX - xDist), static_cast<int16_t>(centerY + yDist), 1 ),
           vec3( static_cast<int16_t>(centerX + xDist), static_cast<int16_t>(centerY + yDist), 1 )
       };

       return result;

    }


    void convertToCameraCoordinates( std::vector<vec3> &vertices, uint16_t screenWidth, uint16_t screenHeight ) {
        for ( auto it = vertices.begin(); it != vertices.end(); it++ ) {
            vec3 transformed;
            transformed.x = 2 * (static_cast<float>(it->x) / screenWidth) - 1;
            transformed.y = 2 * (static_cast<float>(screenHeight - it->y) / screenHeight) - 1;
            transformed.z = 1;

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

