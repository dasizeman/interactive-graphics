#include <vector>
#include <memory>
#include <iostream>

#include "object.hpp"
#include "scene.hpp"
#include "dice_roller_scene.hpp"
#include "cube.hpp"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveything the user sees and interacts with    
    std::shared_ptr<Scene> scene(new DiceRollerScene());

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    /*
    scene->addEntity(std::unique_ptr<Entity>(new LightedRecursiveSphere(-2, 0.95, -4, 5)));
    scene->addEntity(std::unique_ptr<Entity>(new TexturedLightedCube(2, 0.5, -4)));

    scene->addEntity(std::unique_ptr<Entity>(new TexturedLightedPlane(0, 0, 0, 0, 0)));


    scene->addEntity(std::unique_ptr<Entity>(new TexturedSkyPlane(0, 50, 0, 0, 0)));
    scene->addEntity(std::unique_ptr<Entity>(new TexturedSkyPlane(-25, 25, 0, 0, 90)));
    scene->addEntity(std::unique_ptr<Entity>(new TexturedSkyPlane(25, 25, 0, 0, 90)));
    scene->addEntity(std::unique_ptr<Entity>(new TexturedSkyPlane(0, 25, -25, 90, 90)));
    scene->addEntity(std::unique_ptr<Entity>(new TexturedSkyPlane(0, 25, 25, 90, 90)));
    */


    scene->addEntity(std::unique_ptr<Entity>(new Cube(2, 0.5, -4, 0, 0, 0)));


    scene->start();

    return 0;
}
