#include <set>
#include <algorithm>
#include "ember/collision/CollisionEngine.hpp"
#include "ember/collision/Collider.hpp"
#include "ember/core/Scene.hpp"

using namespace ember::collision;

void CollisionEngine::TriggerCollisions() {
    using CollisionPair = std::pair<Behaviour::id, Behaviour::id>;
    std::set<CollisionPair> _already_collided;
    std::vector<std::shared_ptr<BaseCollider>> traversal_copy(_movable_colliders);
    for (const auto& movable_collider : traversal_copy) {
        for (const std::weak_ptr<BaseCollider>& to_collide_with : GetCollisionShortlistForCollider(movable_collider)) {
            auto to_collide = to_collide_with.lock();
            if((to_collide != nullptr) && movable_collider->CollidesWith(to_collide) &&
            _already_collided.count(CollisionPair(to_collide->behaviour_id(), movable_collider->behaviour_id())) == 0) {
                movable_collider->CastCollisionEvent(to_collide);
                to_collide->CastCollisionEvent(movable_collider);
                _already_collided.insert(CollisionPair(movable_collider->behaviour_id(), to_collide->behaviour_id()));
            }
        }
    }
}

std::vector<std::weak_ptr<BaseCollider>> CollisionEngine::GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->GetPotentiallyCollidingWith(collider);
    } else {
        std::vector<std::weak_ptr<BaseCollider>> result;
        for (const auto& static_collider : _static_colliders) {
            if (static_collider->is_attached()) {
                result.push_back(static_collider);
            }
        }
        for (const auto& _movable_collider : _movable_colliders) {
            if (_movable_collider->is_attached() && collider->behaviour_id() != _movable_collider->behaviour_id()) {
                result.push_back(_movable_collider);
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
        _static_colliders.push_back(shared_collider);
    } else {
        _movable_colliders.push_back(shared_collider);
    }

    if (_spatial_partitioner) {
        _spatial_partitioner->PartitionCollider(collider);
    }
}

void CollisionEngine::UnregisterCollider(const std::weak_ptr<BaseCollider>& collider) {
    auto shared_collider = collider.lock();
    auto behaviour_id = shared_collider->behaviour_id();
    if (shared_collider->IsStatic()) {
        auto it = std::remove_if(_static_colliders.begin(), _static_colliders.end(), [behaviour_id](const std::shared_ptr<BaseCollider>& elem) {
            return elem->behaviour_id() == behaviour_id;
        });
        _static_colliders.erase(it, _static_colliders.end());
    } else {
        auto it = std::remove_if(_movable_colliders.begin(), _movable_colliders.end(), [behaviour_id](const std::shared_ptr<BaseCollider>& elem) {
            return elem->behaviour_id() == behaviour_id;
        });
        _movable_colliders.erase(it, _movable_colliders.end());
    }

    if (_spatial_partitioner) {
        _spatial_partitioner->RemovePartitionedCollider(collider);
    }
}
