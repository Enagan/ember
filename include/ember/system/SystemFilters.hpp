#ifndef Ember_Requires_hpp
#define Ember_Requires_hpp

#include <functional>
#include <deque>
#include "ember/core/GameObject.hpp"

namespace ember {
namespace system {

template <class... RequiredBehaviours>
struct RequiresBehaviours {
public:
    static constexpr const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static const auto filter = [](const GameObject& object) {
            return InternalFilterForExcludes(object, PolymorphicRequiresBehaviours());
        }
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr TypeList InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            InternalFilterForExcludes(object, RequiresBehaviours<Args...>());
        } else {
            return false;
        }
    }
};

template <class... RequiredBehaviours>
struct PolymorphicRequiresBehaviours {
public:
    static constexpr const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static const auto filter = [](const GameObject& object) {
            return InternalFilterForChilds(object, PolymorphicRequiresBehaviours());
        }
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject& object, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr TypeList InternalFilterForChilds(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() != 0;
        if (passes_filter_for_t) {
            InternalFilterForChilds(object, RequiresBehaviours<Args...>());
        } else {
            return false;
        }
    }
};

template <class... RequiredBehaviours>
struct ExcludeBehaviours {
public:
    static constexpr const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static const auto filter = [](const GameObject& object) {
            return InternalFilterForExcludes(object, PolymorphicRequiresBehaviours());
        }
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr TypeList InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = !object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            InternalFilterForExcludes(object, RequiresBehaviours<Args...>());
        } else {
            return false;
        }
    }
};

template <class... RequiredBehaviours>
struct PolymorphicExcludeBehaviours {
public:
    static constexpr const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static const auto filter = [](const GameObject& object) {
            return InternalFilterForChilds(object, PolymorphicRequiresBehaviours());
        }
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject& object, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr TypeList InternalFilterForChilds(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() == 0;
        if (passes_filter_for_t) {
            InternalFilterForChilds(object, RequiresBehaviours<Args...>());
        } else {
            return false;
        }
    }
};

template <class... RequiredBehaviours>
struct CompositeFilter {
public:
    static constexpr const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static const auto filter = [](const GameObject& object) {
            for (const auto& filter : GetFilterFunctions()) {
                if (!filter(object)) {
                    return false;
                }
            }
            return true;
        }
    }

private:
    using FilterFunList = std::deque<std::function<bool(const GameObject& object)>>;

    static constexpr const FilterFunList& GetFilterFunctions() {
        static const FilterFunList functions = InternalGetFilterFunctions(RequiresBehaviours());
        return functions;
    }

    template <class... Args>
    static constexpr FilterFunList InternalGetFilterFunctions(RequiresBehaviours<Args...>) {
        return FilterFunList();
    }

    template <class T, class... Args>
    static constexpr FilterFunList InternalGetFilterFunctions(RequiresBehaviours<T, Args...>) {
        TypeList rest_type_list = InternalGetFilterFunctions(RequiresBehaviours<Args...>());
        rest_type_list.push_front(T::GetFilterFun());
        return rest_type_list;
    }
};

}
}

#endif
