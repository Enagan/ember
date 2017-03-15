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
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForExcludes(object, RequiresBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject&, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            return InternalFilterForExcludes(object, RequiresBehaviours<Args...>());
        }
        return false;
    }
};

template <class... RequiredBehaviours>
struct PolymorphicRequiresBehaviours {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForChilds(object, PolymorphicRequiresBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject&, PolymorphicRequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject& object, PolymorphicRequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() != 0;
        if (passes_filter_for_t) {
            return InternalFilterForChilds(object, PolymorphicRequiresBehaviours<Args...>());
        }
        return false;
    }
};

template <class... RequiredBehaviours>
struct ExcludeBehaviours {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForExcludes(object, ExcludeBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject&, ExcludeBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr bool InternalFilterForExcludes(const GameObject& object, ExcludeBehaviours<T, Args...>) {
        bool passes_filter_for_t = !object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            return InternalFilterForExcludes(object, ExcludeBehaviours<Args...>());
        }
        return false;
    }
};

template <class... RequiredBehaviours>
struct PolymorphicExcludeBehaviours {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForChilds(object, PolymorphicExcludeBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject&, PolymorphicExcludeBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static constexpr bool InternalFilterForChilds(const GameObject& object, PolymorphicExcludeBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() == 0;
        if (passes_filter_for_t) {
            return InternalFilterForChilds(object, PolymorphicExcludeBehaviours<Args...>());
        }
        return false;
    }
};

template <class... RequiredBehaviours>
struct CompositeFilter {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            for (const auto& filter : GetFilterFunctions()) {
                if (!filter(object)) {
                    return false;
                }
            }
            return true;
        };
        return filter;
    }

private:
    using FilterFunList = std::deque<std::function<bool(const GameObject& object)>>;

    static const FilterFunList& GetFilterFunctions() {
        static const FilterFunList functions = InternalGetFilterFunctions(CompositeFilter());
        return functions;
    }

    template <class... Args>
    static FilterFunList InternalGetFilterFunctions(CompositeFilter<Args...>) {
        return FilterFunList();
    }

    template <class T, class... Args>
    static FilterFunList InternalGetFilterFunctions(CompositeFilter<T, Args...>) {
        FilterFunList rest_type_list = InternalGetFilterFunctions(CompositeFilter<Args...>());
        rest_type_list.push_front(T::GetFilterFun());
        return rest_type_list;
    }
};

}
}

#endif
