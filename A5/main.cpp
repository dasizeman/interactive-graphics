#include <vector>
#include <memory>
#include <iostream>

#include "geometry.hpp"
#include "scene.hpp"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveything the user sees and interacts with    
    std::shared_ptr<Scene> scene(new A5Scene());

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    scene->addEntity(std::unique_ptr<Entity>(new LightedRecursiveSphere(0, 1, -4, 5)));
    scene->addEntity(std::unique_ptr<Entity>(new LightedPlane()));
    scene->addEntity(std::unique_ptr<Entity>(new LightedCube(0, 1, 4)));


    scene->start();

    return 0;
}
