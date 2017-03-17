#ifndef Ember_BaseSystem_hpp
#define Ember_BaseSystem_hpp

#include <map>
#include <vector>
#include <memory>

#include "ember/core/GameObject.hpp"

namespace ember {
class BaseSystem {
    friend class Scene;
public:
    using SystemFilter = std::function<bool(const GameObject& object)>;

    BaseSystem(SystemFilter filter) : _filter_fun(std::move(filter)) {}
    BaseSystem(const BaseSystem& other) = delete;
    BaseSystem& operator=(const BaseSystem& other) = delete;

// Overrideable
public:
    virtual void onStart() {}

    // --- Basic System Usage
    // Override these methods if your system simply has to do a specific job on each of it's filtered gameobjects and nothing more
    virtual void onPreUpdate(GameObject& /*object*/) {}
    virtual void onUpdate(double /*deltaT*/, GameObject& /*object*/) {}
    virtual void onPostUpdate(GameObject& /*object*/) {}

    // --- Advanced System Usage
    // Override these if you want to directly the entire flow of the system (potentially not iterating over all filtered objects, taking action on the set of object as a whole, etc...)
    // including GameObject addition and removal
    virtual void onPreUpdate();
    virtual void onUpdate(double deltaT);
    virtual void onPostUpdate();
protected:
    virtual void onGameObjectAdded(const std::shared_ptr<GameObject>& shared_obj);
    virtual void onGameObjectRemoved(const std::shared_ptr<GameObject>& shared_obj);

// Internal
public:
    inline ember::Scene& scene() { return *_parent_scene; }
    inline const ember::Scene& scene() const { return *_parent_scene; }

protected:
    void FilterGameObject(const std::shared_ptr<GameObject>& shared_obj);

private:
    // Weak pointer to the scene the gameobject is attached to. If the system exists
    // the scene WILL exist.
    class ember::Scene* _parent_scene = nullptr;
    SystemFilter _filter_fun;

protected:
    std::map<GameObject::id, std::weak_ptr<GameObject>> _managed_objects;
};

}
#endif
