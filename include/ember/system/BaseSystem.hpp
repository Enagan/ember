#ifndef Ember_BaseSystem_hpp
#define Ember_BaseSystem_hpp

#include "ember/system/TypeFilter.hpp"

namespace ember {
namespace system {
class BaseSystem {
public:
    BaseSystem(TypeFilter filter) : _filter(std::move(filter)) {}
    inline const TypeFilter& GetFilter() const { return _filter; }
private:
    TypeFilter _filter;
};

}
}
#endif
