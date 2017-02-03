#include <vector>
#include <memory>
#include <iostream>

#include "2d.h"
#include "scene.h"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );
    std::vector<vec3> points = {
        vec3( 0.25, 0.25, 1),
        vec3( 0.75, 0.25, 1),
        vec3( 0.75, 0.75, 1),
        vec3( 0.25, 0.75, 1),
    };

    std::vector<vec4> colors = {
         vec4( 0.0, 0.0, 1.0, 1.0 ),
         vec4( 0.0, 0.0, 1.0, 1.0 ),
         vec4( 0.0, 0.0, 1.0, 1.0 ),
         vec4( 0.0, 0.0, 1.0, 1.0 ),
    };

    Scene::getInstance().addEntity(std::unique_ptr<Entity>(new Polygon(points, colors)));

	Scene::getInstance().start();


    return 0;
}
