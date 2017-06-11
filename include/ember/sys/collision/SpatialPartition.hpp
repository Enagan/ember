#ifndef Ember_SpatialPartition_hpp
#define Ember_SpatialPartition_hpp

#include <memory>
#include <vector>

namespace ember {
    class Scene;
namespace sys {
namespace collision {

/// A Spatial Partition is a subclass of the abstract class presented here, that can be used as a performance addon attached to the Collision Engine.
/// It simply must follow the presented API, and it's goal is to provide the Engine with a culled collision shortlist for every movable collider existing in the scene.
class SpatialPartition {
    friend class CollisionEngine;

public:
    ember::Scene& scene();

public:
    /// Must be overriden by subclass

    /// Called from the Collision Engine, whenever a new collidable game object is present in the scene.
    virtual void PartitionCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;

    /// Called from the Collision Engine, whenever a collidable game object is removed from the scene.
    virtual void RemovePartitionedCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;

    /// Called from the Collision Engine, to update all movable collidable game objects in the partition before a collision pass.
    virtual void UpdatePartitionedCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;

    /// Called for every movable collider during a collision pass. Should return a shortlist of the colliding objects that are likely to be colliding
    /// with collider
    virtual std::vector<std::weak_ptr<class BaseCollider>> GetPotentiallyCollidingWith(const std::weak_ptr<class BaseCollider>& collider) = 0;

    // Private members of SpatialPartition contained in header
    #include "_priv/SpatialPartition_priv.hpp"
};
}
}
}

#endif
