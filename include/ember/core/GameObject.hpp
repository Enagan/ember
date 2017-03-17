#ifndef GameObject_hpp
#define GameObject_hpp

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "ember/addons/ListensTo.hpp"
#include "ember/addons/Serializable.hpp"

namespace ember {
class Behaviour;
class Scene;
class GameObject {
    friend class Scene;
public:
    using id = std::size_t;
public:
	GameObject();
	GameObject(GameObject&& other);
	GameObject(const GameObject& other) = delete;
    GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other);
	~GameObject();
public:
	void onStart();
    void onPreUpdate();
    void onUpdate(double deltaT);
    void onPostUpdate();

public:
    inline GameObject::id object_id() const { return _id; }
    inline Scene& scene() { return *_parent_scene; }
    inline const Scene& scene() const { return *_parent_scene; }

    /// Adds a behaviour to the GameObject. The Behaviour is emplaced within the game object itself.
    /// A Behaviour can be any class that derives from Behaviour.hpp, but should not be the base heaviour class itself.
    /// Only one behaviour of each type may reside inside a GameObject. If a second one is atempted to added, the function will have no effect.
	template <typename BehaviourSubType, typename... Args>
	GameObject& withBehaviour(Args&&... args);

    /// Checks if the Game Object has a behaviour of the specified type (non-polymorphic, only accounts for behaviours of the exact type provided)
    template <typename BehaviourSubType>
	bool hasBehaviour() const;

    /// Fetches a non-polymorphic reference to a contained behaviour in the GameObject.
    /// Throws an exception if no such behaviour is present.
    /// Usefull to quickly access the behaviour as an expression, but unsafe to keep, as it may be unexpectedly destroyed.
    template <typename BehaviourSubType>
	BehaviourSubType& refBehaviour() throw(std::invalid_argument);
    template <typename BehaviourSubType>
	const BehaviourSubType& refBehaviour() const throw(std::invalid_argument);

    /// Gets a non-polymorphic weak pointer to a contained behaviour in the GameObject.
    /// Returns expired weak_ptr if no such behaviour is present.
    /// Usefull if you need to keep a safe pointer to the behaviour, to check if it still exists,
    ///  or to potentially lock it for access post GameObject destruction.
    template <typename BehaviourSubType>
	std::weak_ptr<BehaviourSubType> getBehaviour();
    template <typename BehaviourSubType>
	std::weak_ptr<const BehaviourSubType> getBehaviour() const;

    /// Gets all weak pointers of behaviours that either are a BehaviourType or subclasses of it.
    /// Usefull if you need to fetch all behaviours with a common parent, such as behaviour addons (for example, SerializableInto's).
    /// This method is expensive to perform, as it will have to go through all behaviours in the GameObject, use refComponent or getComponent
    ///   if you are trying to reach a specific component.
    template <typename BehaviourType>
	std::vector<std::weak_ptr<BehaviourType>> getBehaviours();
    template <typename BehaviourType>
    std::vector<std::weak_ptr<const BehaviourType>> getBehaviours() const;

    /// Triggers an event through all the child behaviours that have the ListenTo Addon (are subclasses of ListenTo<EventType>)
    template <typename EventType>
	void CastEvent(const EventType& event);

    /// Calls SerializeInto or PartialSerializeInto on all child behaviours that have the SerializableInto Addon
    /// (are subclasses of SerializableInto) The resulting object into, will be cached, as such, further calls to this function
    /// will return the cached instance of SerializableInto& into. As such, the input parameter type must implement a valid copy
    /// constructor
    template <typename SerializableInto>
    bool SerializeInto(SerializableInto& into);
    template <typename SerializableInto>
    bool PartialSerializeInto(SerializableInto& into);

    /// Removes self from scene and deallocates
    void Destroy();

private:
    void CheckForCacheInvalidation();

private:
    GameObject::id _id = 0;
	bool _hasStarted{ false };
    std::size_t _next_behaviour_index = 0;
	std::unordered_map<std::type_index, std::shared_ptr<Behaviour>> _behaviours;
    bool _behaviours_changed = false;

    struct SerializedCacheBase;
    template <typename SerializableType> struct SerializedCacheSub;
    std::unordered_map<std::type_index, std::unique_ptr<SerializedCacheBase>> _serialization_cache;
    std::unordered_map<std::type_index, std::unique_ptr<SerializedCacheBase>> _partial_serialization_cache;

    // Weak pointer to the scene the gameobject is attached to. If the gameobject exists
    // The scene WILL exist
    class Scene* _parent_scene = nullptr;
};
}
#include "_impl/GameObject_impl.hpp"

#endif
