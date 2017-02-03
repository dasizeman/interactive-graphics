#include <vector>
#include <memory>
#include <iostream>

#include "2d.h"
#include "scene.h"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

	Scene::getInstance().start();

	std::cout << "Hello!" << std::endl;

    return 0;
}
