#include <string>
#include <iostream>
#include "ember/core/Scene.hpp"
#include "ember/core/GameObject.hpp"

int main(int, const char *[]){
    ember::Scene scene;
    std::cout << scene.addGameObject().object_id() << std::endl;
    return 0;
}
