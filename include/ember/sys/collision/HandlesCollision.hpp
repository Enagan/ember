#ifndef Ember_CollisionEvent_hpp
#define Ember_CollisionEvent_hpp

#include "_priv/Collision.hpp"

namespace ember {
namespace sys {
namespace collision {

/// Handles Collision Behaviour add on can be attached to any Behaviour (by making it subclass from this) and will enable that behaviour to
/// receive events whenever there's a collision between a 'MyCollider' with any collider of type 'OtherCollider'.
/// Only 'MyColliders' present in the same GameObject this behaviour is present on will trigger the event.
template <typename MyCollider, typename OtherCollider = class BaseCollider>
class HandlesCollision : public addons::ListensTo<Collision<MyCollider>> {
public:

    /// Must be overritten on behaviour that implements the addon, will be called whenever the collision event is triggered.
    virtual void HandleCollisionWith(const std::shared_ptr<OtherCollider>& collided_with) = 0;

    // Private members of HandlesCollision contained in header
    #include "_priv/HandlesCollision_priv.hpp"
};

}
}
}
#endif
