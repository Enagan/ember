#ifndef Ember_SpatialPartition_hpp
#define Ember_SpatialPartition_hpp

#include <memory>
#include <vector>

namespace ember {
    class Scene;
namespace collision {
class SpatialPartition {
    friend class CollisionEngine;
public:
    virtual void PartitionCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;
    virtual void RemovePartitionedCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;
    virtual void UpdatePartitionedCollider(const std::weak_ptr<class BaseCollider>& collider) = 0;
    
    virtual std::vector<std::weak_ptr<class BaseCollider>> GetPotentiallyCollidingWith(const std::weak_ptr<class BaseCollider>& collider) = 0;

    ember::Scene& scene();
    
private:
    class CollisionEngine* _owning_engine;
};
}
}
#endif