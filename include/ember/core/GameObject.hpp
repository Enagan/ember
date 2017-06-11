#ifndef GameObject_hpp
#define GameObject_hpp

#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "ember/addons/ListensTo.hpp"
#include "ember/addons/Serializable.hpp"

namespace ember {
class Behaviour;
class Scene;

/// The Gameobject is one of the essential building blocks of Ember. In it's essence, it's purpose is to exist as an
/// aggregate of behaviours, modifying itself, and others based solely on the behaviours it contains, and follows.
/// The GameObject class is meant to be used in conjunction with the Scene and Behaviours, but like the Scene, it's should not be subclassed or modified.
class GameObject {
    friend class Scene;
public:
    /// Type that defined the id of a GameObject. Identifies a gameobject as unique inside a scene.
    using id = std::size_t;

public:
	GameObject();
	GameObject(GameObject&& other);
	GameObject(const GameObject& other) = delete;
    GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other);
	~GameObject();

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
    template <typename BehaviourSubType>
	std::vector<std::weak_ptr<BehaviourSubType>> getBehaviours();
    template <typename BehaviourSubType>
    std::vector<std::weak_ptr<const BehaviourSubType>> getBehaviours() const;

    /// Triggers an event through all the child behaviours that have the ListenTo Addon (are subclasses of ListenTo<EventType>)
    template <typename EventType>
	void CastEvent(const EventType& event);

    /// Serializes the gameobject into the object of the data type provided.
    /// Serialization works by going through all of the gameobjects contained behaviours and attemtping to serialize them to this datatype
    /// As long as at least one behaviour is able to serialize itself successfully, this method will return true, and the serializations will be contained in
    /// 'into'.
    /// Serializations are cached until further changes. This means that if this method is called several times, internally,
    /// The behaviours serialization functions will only be called again if the behaviours indicate that something in their internal state has changed.
    template <typename SerializableInto>
    bool SerializeInto(SerializableInto& into);

    /// Equal to the above function, but won't contain the full serialization of the GameObject. Behaviours can define partial serializations (usually only with
    /// mission critical data, or only with a change set since the previous serialization call). This method will call those partial serialization methods.
    template <typename SerializableInto>
    bool PartialSerializeInto(SerializableInto& into);

    /// Removes GameObject from scene and deallocates it.
    /// Keep in mind that Scene guarantees that Object/Behaviour deletion will ONLY happen in between Update Cycle callbacks
    /// what this means, for example, is that it's safe to delete objects at any time inside those callbacks.
    /// In a behaviour, on the 'onPreUpdate', 'onUpdate', 'onPostUpdate' calls, if you call Destroy (or Scene::removeGameObject, which is equivalent)
    /// then you can be sure that the object will only actually be deleted after that update cycle callback itself is called on every other GameObject.
    void Destroy();

    // Private members of GameObject contained in header
    #include "_priv/GameObject_priv.hpp"
};
}
#include "_impl/GameObject_impl.hpp"

#endif
