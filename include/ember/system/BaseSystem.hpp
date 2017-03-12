#ifndef Ember_BaseSystem_hpp
#define Ember_BaseSystem_hpp

namespace ember {
namespace system {
class BaseSystem {
public:
    using SystemFilter = std::function<bool(const GameObject& object)>;

    BaseSystem(SystemFilter filter) : _filter_fun(std::move(filter)) {}
    BaseSystem(const BaseSystem& other) = delete;
    BaseSystem& operator=(const BaseSystem& other) = delete;

public:
    virtual void onStart() = 0;
    virtual void onPreUpdate() = 0;
    virtual void onUpdate(double deltaT) = 0;
    virtual void onPostUpdate() = 0;

protected:
    inline Scene& scene() { return *_parent_scene; }
    inline const Scene& scene() const { return *_parent_scene; }

private:
    // Weak pointer to the scene the gameobject is attached to. If the system exists
    // the scene WILL exist.
    class Scene* _parent_scene = nullptr;

    SystemFilter _filter_fun;
};

}
}
#endif
