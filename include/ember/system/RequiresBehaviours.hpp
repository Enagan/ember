#ifndef Ember_Requires_hpp
#define Ember_Requires_hpp

#include "TypeList.hpp"

namespace ember {
namespace system {

template <class... RequiredBehaviours>
struct RequiresBehaviours {
public:
    static constexpr TypeList GetTypeList() {
        return InternalGetTypeList(RequiresBehaviours());
    }

private:
    template <class... Args>
    static constexpr TypeList InternalGetTypeList(RequiresBehaviours<Args...>) {
        return TypeList();
    }

    template <class T, class... Args>
    static constexpr TypeList InternalGetTypeList(RequiresBehaviours<T, Args...>) {
        TypeList rest_type_list = InternalGetTypeList(RequiresBehaviours<Args...>());
        rest_type_list.insert(std::type_index(typeid(T)));
        return rest_type_list;
    }
};

}
}

#endif
