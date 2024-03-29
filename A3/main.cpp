#include <vector>
#include <memory>
#include <iostream>

#include "geometry.hpp"
#include "scene.hpp"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveythung the user sees and interacts with    
    std::shared_ptr<Scene> scene(new A3Scene());

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    // Create a cube
    scene->addEntity(std::unique_ptr<Entity>(new Model(0, 0, -2, 8, 0.5, 1.0)));

    scene->start();

    return 0;
}
