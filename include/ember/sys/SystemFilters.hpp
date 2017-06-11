#ifndef Ember_Requires_hpp
#define Ember_Requires_hpp

#include <functional>
#include <deque>
#include "ember/core/GameObject.hpp"

namespace ember {
namespace sys {

/// System Filters available in this header are to be used as the template parameter in Systems (System.hpp).
/// They Provide a way to filter GameObject into a system, primarily through Type checks.
/// You can write your own SystemFilter if you wish, as long as they follow the API presented in struct 'SystemFilter'.
struct SystemFilter {
    static const std::function<bool(const GameObject& object)>& GetFilterFun();
};


/// RequiresBehaviour is a variadic template filter that ensure that all GameObjects in the System will have all the behaviours listed.
/// Example: 'class MySystem : public System<RequiresBehaviours<MyBehaviour1, MyBehaviour2, MyBehaviour3>>'. Objects in MySystem will
/// be guaranteed to have the three behaviours.
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
    static bool InternalFilterForExcludes(const GameObject&, RequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static bool InternalFilterForExcludes(const GameObject& object, RequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            return InternalFilterForExcludes(object, RequiresBehaviours<Args...>());
        }
        return false;
    }
};

/// PolymorphicRequiresBehaviours is a variadic template filter that ensure that all GameObjects in the System will have
/// all the behaviours listed, or subclasses of those behaviours.
/// Example: 'class MySystem : public System<PolymorphicRequiresBehaviours<MyBehaviour1>>'. Objects in MySystem will be
/// guaranteed to have either MyBehaviour1, or 'class MySubBehaviour1 : public MyBehaviour1', etc...
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
    static bool InternalFilterForChilds(const GameObject&, PolymorphicRequiresBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static bool InternalFilterForChilds(const GameObject& object, PolymorphicRequiresBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() != 0;
        if (passes_filter_for_t) {
            return InternalFilterForChilds(object, PolymorphicRequiresBehaviours<Args...>());
        }
        return false;
    }
};

/// ExcludesBehaviours is a variadic template filter that ensure that all GameObjects in the System will not have
/// any of the behaviours listed.
/// Example: 'class MySystem : public System<ExcludesBehaviours<MyBehaviour1, MyBehaviour2>>'. Objects in MySystem will be
/// guaranteed to have neither MyBehaviour1 or MyBehaviour2.
template <class... RequiredBehaviours>
struct ExcludesBehaviours {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForExcludes(object, ExcludesBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static bool InternalFilterForExcludes(const GameObject&, ExcludesBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static bool InternalFilterForExcludes(const GameObject& object, ExcludesBehaviours<T, Args...>) {
        bool passes_filter_for_t = !object.hasBehaviour<T>();
        if (passes_filter_for_t) {
            return InternalFilterForExcludes(object, ExcludesBehaviours<Args...>());
        }
        return false;
    }
};

/// PolymorphicExcludesBehaviours is a variadic template filter that ensure that all GameObjects in the System will not have
/// any of the behaviours listed, or any subclasses of those behaviours.
/// Example: 'class MySystem : public System<PolymorphicExcludesBehaviours<MyBehaviour1>>'. Objects in MySystem will be
/// guaranteed to have neither MyBehaviour1 or 'class MySubBehaviour1 : public MyBehaviour1', etc...
template <class... RequiredBehaviours>
struct PolymorphicExcludesBehaviours {
public:
    static const std::function<bool(const GameObject& object)>& GetFilterFun() {
        static std::function<bool(const GameObject& object)> filter = [](const GameObject& object) {
            return InternalFilterForChilds(object, PolymorphicExcludesBehaviours());
        };
        return filter;
    }

private:

    template <class... Args>
    static bool InternalFilterForChilds(const GameObject&, PolymorphicExcludesBehaviours<Args...>) {
        return true;
    }

    template <class T, class... Args>
    static bool InternalFilterForChilds(const GameObject& object, PolymorphicExcludesBehaviours<T, Args...>) {
        bool passes_filter_for_t = object.getBehaviours<T>().size() == 0;
        if (passes_filter_for_t) {
            return InternalFilterForChilds(object, PolymorphicExcludesBehaviours<Args...>());
        }
        return false;
    }
};

/// CompositeFilter is a variadic template filter that is used to combine any other SystemFilters together.
/// It guarantees that objects present in the system must be able to pass all filters.
/// Example: 'class MySystem : public System<CompositeFilter<RequiresBehaviours<MyBehaviour1>, ExcludesBehaviours<MyBehaviour2>>'.
/// Objects in MySystem will be guaranteed to have neither MyBehaviour1 while not having MyBehaviour2.
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
