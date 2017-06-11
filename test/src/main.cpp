#include <string>
#include <iostream>
#include "ember/core/Scene.hpp"
#include "ember/core/GameObject.hpp"

#include "ember/core/System.hpp"
#include "ember/core/Behaviour.hpp"

class LolBehaviour : public ember::Behaviour {
public:
    LolBehaviour(int coiso) : _aquela_base(coiso) {}
    int _aquela_base;
};

class LulBehaviour : public LolBehaviour {
public:
    LulBehaviour(int coiso) : LolBehaviour(coiso) {}
};

class LelBehaviour : public ember::Behaviour {
public:
    LelBehaviour(int coiso) : _aquela_base(coiso) {}
    int _aquela_base;
};

class HueHueHueBehaviour : public ember::Behaviour {
public:
    HueHueHueBehaviour(int coiso) : _aquela_base(coiso) {}
    int _aquela_base;
};

class Laughsys : public ember::System<ember::sys::CompositeFilter<
    ember::sys::PolymorphicRequiresBehaviours<LolBehaviour>
    ,ember::sys::RequiresBehaviours<LelBehaviour>
    ,ember::sys::ExcludesBehaviours<HueHueHueBehaviour>>>
    //,ember::System::PolymorphicExcludesBehaviours<LolBehaviour>
    {
    virtual void onUpdate(double, ember::GameObject& object) override {
        if (object.hasBehaviour<LolBehaviour>()) {
            std::cout << "Lols: " << object.refBehaviour<LolBehaviour>()._aquela_base << std::endl;
        } else if(object.hasBehaviour<LulBehaviour>()) {
            std::cout << "Luls: " << object.refBehaviour<LulBehaviour>()._aquela_base << std::endl;
        } else if(object.hasBehaviour<LelBehaviour>()) {
            std::cout << "Lels: " << object.refBehaviour<LelBehaviour>()._aquela_base << std::endl;
        }
    }
};

int main(int, const char *[]){
    ember::Scene scene;
    scene.attachSystem<Laughsys>();
    for (size_t i = 0; i < 1; i++) {
        std::cout << scene.addGameObject().withBehaviour<LolBehaviour>(1000+i).object_id() << std::endl;
    }

    for (size_t i = 0; i < 1; i++) {
        std::cout << scene.addGameObject().withBehaviour<LelBehaviour>(2000+i).object_id() << std::endl;
    }

    for (size_t i = 0; i < 1; i++) {
        std::cout << scene.addGameObject().withBehaviour<LelBehaviour>(3000+i).withBehaviour<LolBehaviour>(3000+i).object_id() << std::endl;
    }

    for (size_t i = 0; i < 1; i++) {
        std::cout << scene.addGameObject().withBehaviour<LolBehaviour>(4000+i).withBehaviour<HueHueHueBehaviour>(4000+i).object_id() << std::endl;
    }

    for (size_t i = 0; i < 1; i++) {
        std::cout << scene.addGameObject().withBehaviour<LulBehaviour>(5000+i).withBehaviour<LelBehaviour>(5000+i).object_id() << std::endl;
    }

    scene.RunUpdateCycle(3);
    return 0;
}
