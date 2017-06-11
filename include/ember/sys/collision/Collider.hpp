#ifndef Ember_Collider_hpp
#define Ember_Collider_hpp

#include "ember/core/Behaviour.hpp"
#include "ember/core/GameObject.hpp"
#include "_priv/Collision.hpp"

namespace ember {
namespace sys {
namespace collision {

#include "_priv/BaseCollider_priv.hpp"

/// A Central piece of the Collision Engine customizability is via the Collider Behaviour.
/// CollisionEngine users should create their own custom colliders for their game, as subclasses of this class. (AABB Collider, Circle Collider, etc...)
/// The Collider uses the 'Curiously Recurring Template' design pattern, this means that if you make a Collider, it's inheritance should look like this:
/// 'class MyCollider : public Collider<MyCollider> {...};'
/// On your sub collider class, the method CollidesWith must be implemented, returning true if a collision is happening.
/// Since Colliders are subclasses of Behaviours (through BaseCollider), all applicable Behaviour overrides are available

template<typename ColliderSubType>
class Collider : public BaseCollider {
    friend class CollisionEngine;
public:
    /// Colliders can be marked as static. What this will mean to the engine is that this collider will never have to be updated, after creation.
    /// set static to true if your collider will never move (or otherwise change), and the engine will optimize accordingly.
    Collider(bool is_static) : BaseCollider(is_static) {}

    /// If this collider and 'other' are colliding, whatever that means on your subclass, then this method should return true.
    virtual bool CollidesWith(const std::shared_ptr<BaseCollider>& other) override = 0;

    /// If you need to override onEnd, make sure to call the Base class onEnd, or behaviour of the CollisionEngine will be affected.
    virtual void onEnd() override { BaseCollider::onEnd(); }

    // Private members of Collider contained in header
    #include "_priv/Collider_priv.hpp"
};

}
}
}
#endif
