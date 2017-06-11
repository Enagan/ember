#ifndef Ember_BaseSystem_hpp
#define Ember_BaseSystem_hpp

#include <map>
#include <vector>
#include <memory>

#include "ember/core/GameObject.hpp"
#include "ember/sys/SystemFilters.hpp"

namespace ember {

/// The BaseSystem is the Foundation for a System in Ember. It's not the class you should subclass (you can, but your life will be easier
/// if you subclass via the 'System' class, defined below).
/// BaseSystem however defines the API every system must use, providing a few different approaches of varying complexity when writting custom systems.
class BaseSystem {
    friend class Scene;
public:
    using SystemFilter = std::function<bool(const GameObject& object)>;

    BaseSystem(SystemFilter filter) : _filter_fun(std::move(filter)) {}
    BaseSystem(const BaseSystem& other) = delete;
    BaseSystem& operator=(const BaseSystem& other) = delete;

public:
    inline ember::Scene& scene() { return *_parent_scene; }
    inline const ember::Scene& scene() const { return *_parent_scene; }

protected:
    // Will be called by Scene, Should be Overriden on subclasses.

    /// Will be called when the Scene starts. Should be used for any initial setup on the system that depends on having a Scene connection.
    virtual void onStart() {}
    /// Will be called when the Scene will be destroyed. Should be used for any necessary resource deallocation that depends on having a scene connection.
    virtual void onEnd() {}

    // --- Basic System Usage
    // Override these methods if your system simply has to do a specific job on each of it's filtered gameobjects. If you don't need to correlate multiple objects in the system, and if all
    // your action are on an object by object basis, use these overrides.

    /// Will be called at the start of the Scene Update Cycle. Should be used to prepare the objects in the system for the incoming update (clearing 'per cycle' flags for example).
    virtual void onPreUpdate(GameObject& /*object*/) {}

    /// The central function of the update cycle, should be responsible for enacting whatever actions the system exists for. Receives, in milliseconds, how much time has passed since the last update cycle.
    virtual void onUpdate(double /*deltaT*/, GameObject& /*object*/) {}

    /// Will be called at the end of the Scene Update Cycle. Should be used to deal with the outcomes of the update, or to cleanup after any loose ends after the update call.
    virtual void onPostUpdate(GameObject& /*object*/) {}


    // --- Advanced System Usage
    // Override these if you want to directly affect the entire flow of the system (potentially not iterating over all filtered objects, taking action on the set of object as a whole, etc...)
    // Override these if the actions of this system are not necessarily on each of it's objects individually.
    // Keep in mind that if you don't call the base system implementations the _managed_objects container won't be automatically cleaned of no longer valid weak_ptrs. Either call the base methods, or enact the
    // cleanup yourself, as your logic allows.

    /// Will be called at the start of the Scene Update Cycle. Should be used to prepare the objects in the system for the incoming update (clearing 'per cycle' flags for example).
    virtual void onPreUpdate();

    /// The central function of the update cycle, should be responsible for enacting whatever actions the system exists for. Receives, in milliseconds, how much time has passed since the last update cycle.
    virtual void onUpdate(double deltaT);

    /// Will be called at the end of the Scene Update Cycle. Should be used to deal with the outcomes of the update, or to cleanup after any loose ends after the update call.
    virtual void onPostUpdate();

    // --- Advanced Insertion and Deletion of GameObjects
    /// If your system has special requirements for storing GameObjects (you want to use a container other than _managed_objects bellow) override these methods.

    /// onGameObjectAdded will be called every time an object is successfully filtered into the system, either as it is added, or as it's contained behaviours change.
    /// Keep in mind that if you're not using _managed_objects then this can be called more than once for the same object (if, after being filtered into the system, the object changes, but is still system valid).
    virtual void onGameObjectAdded(const std::weak_ptr<GameObject>& weak_obj);

    /// onGameObjectRemoved will be called every time an object fails to pass the filter into the system. Either on the first time it's attempted to be filtered, or if the object changes behaviours and is no longer system valid.
    virtual void onGameObjectRemoved(const std::weak_ptr<GameObject>& weak_obj);


protected:
    std::map<GameObject::id, std::weak_ptr<GameObject>> _managed_objects;

    /// Internal
    #include "_priv/System_priv.hpp"
};


/// The System is a subclass of base system that receives a filter through a Template parameter. It's most useful in conjunction with the Filters defined in SystemFilters.hpp to easily define the restrictions of
/// your custom system right on their inheritance declaration. In all other things, System behaves just like the base system.
template <typename Filter>
class System : public BaseSystem {
    friend class Scene;
public:
    System() : BaseSystem(Filter::GetFilterFun()) {}
};

}
#endif
