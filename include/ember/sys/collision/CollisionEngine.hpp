#ifndef Ember_CollisionEngine_hpp
#define Ember_CollisionEngine_hpp

#include <vector>
#include <memory>
#include "ember/core/System.hpp"
#include "SpatialPartition.hpp"
#include "Collider.hpp"

namespace ember {
namespace sys {
namespace collision {

/// The CollisionEngine is a non-mandatory, prefabricated system to use with your ember scene. It encapsulates common
/// collision computation procedures, while allowing for the project specifics to be fed into the engine to achieve both simple, and
/// very complex collision systems.
/// The Engine works in tandem with the Collider Behaviour (Collider.hpp) and the HandlesCollision Addon (HandlesCollision.hpp) to allow
/// for gameObjects to behave like colliders, and for any Behaviour to listen to collision events.
/// For Optimization purposes, a Spatial Partition subclass (SpatialPartition.hpp) can be attached to the engine, enabling a user written
/// algorithm to pre-partition the collision space so that not all objects try to collide with every other (like a quad tree algorithm, for example).
/// The Collision Engine should not be subclassed, it should be used as is.
class CollisionEngine final : public System<PolymorphicRequiresBehaviours<BaseCollider>> {
    friend class BaseCollider;
public:
    CollisionEngine() = default;
	CollisionEngine(const CollisionEngine& other) = delete;
    CollisionEngine& operator=(const CollisionEngine& other) = delete;

// Collision Engine
public:

    /// Attaches a SpatialPartitioner to the CollisionEngine. The Collision Engine can only have one SpatialPartitioner at a time, so if any other
    /// was already present, it will be deleted and replaced with the new one.
    /// The Spatial partition will be constructed in place inside the CollisionEngine, and its lifecycle will be completely managed by the engine.
    template <typename SpatialPartitionSubType, typename... Args>
	void withSpatialPartitioner(Args&&... args);

    /// Checks if the Spatial Partitioner is of a specific type. Returns false with any template argument if no Spatial Partitioner is present.
    template <typename SpatialPartitionSubType>
	void hasSpatialPartitionerOfType();

    /// Fetches a reference to the Spatial Partitioner of the specified type. Will throw if there is no spatial partioner, of it the existing
    /// partioner is not of the correct type.
    template <typename SpatialPartitionSubType>
	SpatialPartitionSubType& refSpatialPartitionerOfType() throw(std::invalid_argument);

    // Private members of CollisionEngine contained in header
    #include "_priv/CollisionEngine_priv.hpp"
};

}
}
}

#include "_impl/CollisionEngine_impl.hpp"

#endif
