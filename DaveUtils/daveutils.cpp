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


    void convertToCameraCoordinates( std::vector<vec3> &vertices, uint16_t screenWidth, uint16_t screenHeight ) {
	for ( auto it = vertices.begin(); it != vertices.end(); it++ ) {
	    vec3 transformed;
	    transformed.x = 2 * (static_cast<float>(it->x) / screenWidth) - 1;
	    transformed.y = 2 * (static_cast<float>(screenHeight - it->y) / screenHeight) - 1;

	    *it = transformed;
	}
    }


}
