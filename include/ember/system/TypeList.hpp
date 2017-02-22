#ifndef Ember_TypeList_hpp
#define Ember_TypeList_hpp

#include <typeinfo>
#include <typeindex>
#include <set>

namespace ember {
namespace system {
    using TypeList = std::set<std::type_index>;
}
}

#endif
