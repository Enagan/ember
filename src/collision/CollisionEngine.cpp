#include "ember/collision/CollisionEngine.hpp"
#include "ember/collision/Collider.hpp"
#include "ember/core/Scene.hpp"

using namespace ember::collision;
    
void CollisionEngine::TriggerCollisions() {
    using CollisionPair = std::pair<Behaviour::id, Behaviour::id>;
    std::set<CollisionPair> _already_collided;
    std::set<GameObject::id> traversal_copy(_movable_colliders);
    for (const auto& object_id : traversal_copy) {
        if (scene().hasGameObject(object_id)) {
            for(const auto& weak_collider : _owning_scene->getGameObject(object_id).getBehaviours<BaseCollider>()) {
                auto collider = weak_collider.lock();
                for (const std::weak_ptr<BaseCollider>& to_collide_with : GetCollisionShortlistForCollider(collider)) {
                    auto to_collide = to_collide_with.lock();
                    if((to_collide != nullptr) && collider->CollidesWith(to_collide) && _already_collided.count(CollisionPair(to_collide->behaviour_id(), collider->behaviour_id())) == 0) {
                        collider->CastCollisionEvent(to_collide);
                        to_collide->CastCollisionEvent(collider);
                        _already_collided.insert(CollisionPair(collider->behaviour_id(), to_collide->behaviour_id()));
                    }
                }
            }
        }
    }
}

std::vector<std::weak_ptr<BaseCollider>> CollisionEngine::GetCollisionShortlistForCollider(const std::weak_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->GetPotentiallyCollidingWith(collider);
    } else {
        std::vector<std::weak_ptr<BaseCollider>> result;
        for (const auto& object_id : _static_colliders) {
            result.push_back(_owning_scene->getGameObject(object_id).getBehaviour<BaseCollider>());
        }
        for (const auto& object_id : _movable_colliders) {
            if (object_id != collider.lock()->game_object().object_id()) {
                result.push_back(_owning_scene->getGameObject(object_id).getBehaviour<BaseCollider>());
            }
        }
        return result;
    }
}

void CollisionEngine::UpdateSpatialPartition(const std::weak_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->UpdatePartitionedCollider(collider);
    }
}

void CollisionEngine::RegisterCollider(const std::weak_ptr<BaseCollider>& collider) {
    auto shared_collider = collider.lock();
    if (shared_collider->IsStatic()) {
        _static_colliders.insert(shared_collider->game_object().object_id());
    } else {
        _movable_colliders.insert(shared_collider->game_object().object_id());
    }
    
    if (_spatial_partitioner) {
        _spatial_partitioner->PartitionCollider(collider);
    }
}

void CollisionEngine::UnregisterCollider(const std::weak_ptr<BaseCollider>& collider) {
    auto shared_collider = collider.lock();
    if (shared_collider->IsStatic()) {
        _static_colliders.erase(shared_collider->game_object().object_id());
    } else {
        _movable_colliders.erase(shared_collider->game_object().object_id());
    }
    
    if (_spatial_partitioner) {
        _spatial_partitioner->RemovePartitionedCollider(collider);
    }
}