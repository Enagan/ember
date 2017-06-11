#ifndef Ember_Behaviour_hpp
#define Ember_Behaviour_hpp

#include <memory>
#include <cstddef>
#include <stdexcept>

namespace ember {

class GameObject;
class Scene;

/// The Behaviour is the heart of ember, and alongside the Systems, the main customizable part of the Framework.
/// In Using Ember, you are expected to write your own Behaviours (subclasses of this class), to achieve any and every goal you have
/// for your game. (A player controller behaviour, a transform behavior, an AI behaviour, etc...)
/// Behaviours will be given to attached to GameObjects, enabling them to act upon the scene as a whole.
/// Aside from this, Behaviours can also make use of addons (present in the ember/addons folder) which allows them extra functionality,
/// such as event listening, and Serialization.
class Behaviour {
	friend GameObject;
public:
    /// Type that defined the id of a Behaviour. Identifies a behaviour as unique inside a scene.
    using id = std::pair<std::size_t, std::size_t>;

protected:
    // When creating the subclass constructor, keep in mind that it should only be concerned with initializing Behaviour internals.
    // If any setup work depends on other behaviours, it should be done in 'onStart'
    Behaviour() = default;
    Behaviour(const Behaviour& other) = delete;
    Behaviour& operator=(const Behaviour& other) = delete;

protected:
	// Will be called by Scene, should be Overriden on subclasses.

    /// Will be called when the Scene starts. Should be used for any initial setup on the behaviour that requires the behaviour to be fully integrated within the GameObject.
    virtual void onStart(){};

    /// Will be called at the start of the Scene Update Cycle. Should be used to prepare the Behaviour for the incoming update (clearing 'per cycle' flags for example).
    virtual void onPreUpdate(){};

    /// The central function of the update cycle, should be responsible for enacting whatever actions the behaviour exists for. Receives, in milliseconds, how much time has passed since the last update cycle.
	virtual void onUpdate(double /*deltaT*/){};

    /// Will be called at the end of the Scene Update Cycle. Should be used to deal with the outcomes of the update, or to cleanup after any loose ends after the update call.
	virtual void onPostUpdate(){};

    /// Will be called when the owning game object is about to be destroyed, rendering the behaviour as detached, after onEnd returns. A detached behaviour is no longer present in a scene,
    /// and will no longer be able to access it's game_object.
    /// Nor can it be guaranteed that any connections it might have made with scene objects remain available.
    /// In case no extra retained pointers to the behaviour exist outside the owning GameObject, then behaviour will also be destroyed shortly after the call to onEnd returns.
    virtual void onEnd(){};

public:
    inline Behaviour::id behaviour_id() const { return _id; }

    /// Returns true if the Behaviour is still attached to it's owning game object.
    inline bool is_attached() const { return _gameObjectOwner != nullptr; };

    /// Access methods to owning game object
	inline GameObject& game_object() { return *_gameObjectOwner; };
    inline const GameObject& game_object() const { return *_gameObjectOwner; };

    // Private
    #include "_priv/Behaviour_priv.hpp"
};
}
#endif
