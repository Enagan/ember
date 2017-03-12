#ifndef Ember_System_hpp
#define Ember_System_hpp

#include "ember/system/BaseSystem.hpp"

namespace ember {

template <typename RequireList>
class System : public system::BaseSystem {
    friend class Scene;
public:
    System() : BaseSystem(RequireList::GetFilterFun()) {}
};

}

#endif
