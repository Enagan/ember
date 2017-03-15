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

public:
    virtual void onStart() {}
    virtual void onPreUpdate(GameObject& /*object*/) {}
    virtual void onUpdate(double /*deltaT*/, GameObject& /*object*/) {}
    virtual void onPostUpdate(GameObject& /*object*/) {}

    inline ember::Scene& scene() { return *_parent_scene; }
    inline const ember::Scene& scene() const { return *_parent_scene; }

protected:
    void FilterGameObject(const std::shared_ptr<GameObject>& shared_obj);

    void onPreUpdate();
    void onUpdate(double deltaT);
    void onPostUpdate();

private:
    // Weak pointer to the scene the gameobject is attached to. If the system exists
    // the scene WILL exist.
    class ember::Scene* _parent_scene = nullptr;

    SystemFilter _filter_fun;

    std::map<GameObject::id, std::weak_ptr<GameObject>> _managed_objects;
};

}
#endif
