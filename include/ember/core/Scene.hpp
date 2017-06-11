#ifndef Scene_hpp
#define Scene_hpp

#include <map>
#include <memory>
#include <initializer_list>

#include "GameObject.hpp"
#include "System.hpp"
#include <typeindex>

namespace ember {

/// The Scene is a quintissential part of ember. It's withing a scene that everything that the framework has to offer, happens.
/// A Scene, at it's basic level, is a container of both objects and systems, and is responsible for updating both at the right times, in the right order.
class Scene {
public:
	Scene();
    ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other);
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other);

public:
    inline bool hasStarted() { return _hasStarted; }

    /// Runs an update cycle within the Scene, updating all Behaviours and Systems.
    /// On the first call to RunUpdateCycle, onStart will be called, before the regular cycle calls, on all Behaviours and Systems.
    /// A regular update cycle consists of calling 'onPreUpdate' --> 'onUpdate' --> 'onPostUpdate' on all Behaviours and Systems.
    /// Each callback will always be called first on the Behaviours, and then on the Systems.
    void RunUpdateCycle(double deltaT);

    /// Creates a new GameObject in the scene, returning a reference to it, so it can immediatly be modified.
	GameObject& addGameObject();

    /// Checks if an object with the provided id exists in the scene (id can fetched from a GameObject via the method object_id()).
    bool hasGameObject(GameObject::id index) const;

    /// Fetches a reference to the gameobject with the provided id in the scene.
    GameObject& refGameObject(GameObject::id index) throw(std::invalid_argument);

    /// Removes GameObject with id index from the scene, deleting it and it's contained behaviours.
    /// Keep in mind that Scene guarantees that Object/Behaviour deletion will ONLY happen in between Update Cycle callbacks
    /// what this means, for example, is that it's safe to delete objects at any time in those callbacks.
    /// In a behaviour, on the 'onPreUpdate', 'onUpdate', 'onPostUpdate' calls, if you call removeGameObject (or GameObject::Destroy, which is equivalent)
    /// then you can be sure that the object will only actually be deleted after that run cycle callback itself is called on every other GameObject.
    void removeGameObject(GameObject::id index);


    /// Adds a System to the Scene. The System is built inplace within the scene itself.
    /// A System can be any class that derives from System.hpp.
    /// Only one System of each type may reside inside a Scene. If a second one is added, the method will have no effect.
	template <typename SystemSubType, typename... Args>
    void attachSystem(Args&&... args);

    /// Checks if the Scene has a System of the specified type (non-polymorphic, only accounts for systems of the exact type provided)
    template <typename SystemSubType>
	bool hasSystem() const;

    /// Non-polymorphically fetches a reference to a contained System in the Scene.
    /// Throws an exception if no such system is present.
    template <typename SystemSubType>
	SystemSubType& refSystem() throw(std::invalid_argument);

    /// Triggers an event through all Behaviours and Systems that use the ListenTo<EventType> addon.
    template <typename EventType>
	void BroadcastEvent(const EventType& event);

    // Private members of Scene contained in header
    #include "_priv/Scene_priv.hpp"
};
}
#include "_impl/Scene_impl.hpp"

#endif
