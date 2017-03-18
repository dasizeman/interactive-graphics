#include <vector>
#include <memory>
#include <iostream>

#include "object.hpp"
#include "scene.hpp"
#include "dice_roller_scene.hpp"
#include "cube.hpp"
#include "plane.hpp"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveything the user sees and interacts with    
    std::shared_ptr<Scene> scene(new DiceRollerScene("Cool Sizer: Gotta Oops", 1920, 1080));

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    scene->addEntity(std::unique_ptr<Entity>(new Plane(0, 0, 0, 0,0, 0, 100)));
    scene->addEntity(std::unique_ptr<Entity>(new Cube(2, 0.5, -4, 0, 0, 0)));


    scene->start();

    return 0;
}
