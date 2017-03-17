#ifndef Ember_CollisionEngine_hpp
#define Ember_CollisionEngine_hpp

#include <list>
#include "ember/core/GameObject.hpp"
#include "ember/core/System.hpp"
#include "SpatialPartition.hpp"

namespace ember {
namespace system {
namespace collision {

class CollisionEngine : public System<PolymorphicRequiresBehaviours<BaseCollider>> {
    friend class BaseCollider;
public:
    CollisionEngine() = default;
	CollisionEngine(const CollisionEngine& other) = delete;
    CollisionEngine& operator=(const CollisionEngine& other) = delete;

// From System
public:
    virtual void onPreUpdate() override {} // No work to be done on PreUpdate
    virtual void onUpdate(double /*deltaT*/) override;
    virtual void onPostUpdate() override {} // No work to be done onUpdate

protected:
    virtual void onGameObjectAdded(const std::shared_ptr<GameObject>& shared_obj) override;
    virtual void onGameObjectRemoved(const std::shared_ptr<GameObject>&) override {}

// Collision Engine
public:
    template <typename SpatialPartitionSubType, typename... Args>
	void withSpatialPartitioner(Args&&... args);

    template <typename SpatialPartitionSubType>
	void hasSpatialPartitionerOfType();

    template <typename SpatialPartitionSubType>
	SpatialPartitionSubType& refSpatialPartitionerOfType();

private:
    std::vector<std::weak_ptr<BaseCollider>> GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider);

    void UpdateSpatialPartition(const std::weak_ptr<BaseCollider>& collider);
    void UnregisterCollider(const std::weak_ptr<BaseCollider>& collider);

private:
    std::list<std::weak_ptr<BaseCollider>> _static_colliders;
    std::list<std::weak_ptr<BaseCollider>> _movable_colliders;
    std::unique_ptr<SpatialPartition> _spatial_partitioner;
};

}
}
}

#include "_impl/CollisionEngine_impl.hpp"

#endif
