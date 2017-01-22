#ifndef Ember_CollisionEngine_hpp
#define Ember_CollisionEngine_hpp

#include <set>
#include "ember/core/GameObject.hpp"
#include "SpatialPartition.hpp"

namespace ember {
namespace collision {
class CollisionEngine {
    friend class BaseCollider;
    friend class ember::Scene;
public:
    CollisionEngine() = default;
	CollisionEngine(const CollisionEngine& other) = delete;
    CollisionEngine& operator=(const CollisionEngine& other) = delete;
    
    inline Scene& scene() { return *_owning_scene; }
    
    template <typename SpatialPartitionSubType, typename... Args>
	void withSpatialPartitioner(Args&&... args);
    
    template <typename SpatialPartitionSubType>
	void hasSpatialPartitionerOfType();
    
    template <typename SpatialPartitionSubType>
	SpatialPartitionSubType& refSpatialPartitionerOfType();
        
private:
    void RegisterCollider(const std::weak_ptr<BaseCollider>& collider);
    void UnregisterCollider(const std::weak_ptr<BaseCollider>& collider);
    void UpdateSpatialPartition(const std::weak_ptr<BaseCollider>& collider);
    
    std::vector<std::weak_ptr<BaseCollider>> GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider);
    
    void TriggerCollisions();
    
private:
    Scene* _owning_scene;
    
    std::set<GameObject::id> _static_colliders;
    std::set<GameObject::id> _movable_colliders;
    
    std::unique_ptr<SpatialPartition> _spatial_partitioner;
};
}
}

#include "_impl/CollisionEngine_impl.hpp"

#endif