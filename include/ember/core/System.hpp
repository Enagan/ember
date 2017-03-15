#ifndef Ember_System_hpp
#define Ember_System_hpp

#include "ember/core/BaseSystem.hpp"
#include "ember/system/SystemFilters.hpp"

namespace ember {

template <typename RequireList>
class System : public BaseSystem {
    friend class Scene;
public:
    System() : BaseSystem(RequireList::GetFilterFun()) {}
};

}

#endif
