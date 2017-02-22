#ifndef Ember_TypeFilter_hpp
#define Ember_TypeFilter_hpp

#include "TypeList.hpp"

namespace ember {
namespace system {
class TypeFilter {
public:
    TypeFilter(TypeList requires);
    bool PassesFilter(const TypeList& type_list) const;
private:
    TypeList _requires;
};
}
}

#endif
