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


}
