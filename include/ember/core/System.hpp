#ifndef Ember_System_hpp
#define Ember_System_hpp

#include "ember/system/BaseSystem.hpp"
#include "ember/system/Requires.hpp"

namespace ember {

template <typename RequireList>
class System : public system::BaseSystem {
public:
    System() : BaseSystem(RequireList::GetTypeList()){}
};

}

#endif
