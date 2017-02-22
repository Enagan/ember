#include <algorithm>
#include "ember/system/TypeFilter.hpp"

namespace ember {
namespace system {

TypeFilter::TypeFilter(TypeList requires) :
    _requires(std::move(requires)) {}

bool TypeFilter::PassesFilter(const TypeList& type_list) const {
    return std::includes(type_list.begin(), type_list.end(),
                            _requires.begin(), _requires.end());
}

}
}
