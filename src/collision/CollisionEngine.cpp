#include <set>
#include <algorithm>
#include "ember/system/collision/CollisionEngine.hpp"
#include "ember/system/collision/Collider.hpp"
#include "ember/core/Scene.hpp"

using namespace ember::system::collision;

void CollisionEngine::onPostUpdate() {
    using CollisionPair = std::pair<Behaviour::id, Behaviour::id>;
    std::set<CollisionPair> _already_collided;
    for (auto iter = _movable_colliders.begin(); iter != _movable_colliders.end(); ) {
        if (auto movable_collider = iter->lock()) {
            for (const std::weak_ptr<BaseCollider>& to_collide_with : GetCollisionShortlistForCollider(movable_collider)) {
                auto to_collide = to_collide_with.lock();
                if((to_collide != nullptr) && movable_collider->CollidesWith(to_collide) &&
                _already_collided.count(CollisionPair(to_collide->behaviour_id(), movable_collider->behaviour_id())) == 0) {
                    movable_collider->CastCollisionEvent(to_collide);
                    to_collide->CastCollisionEvent(movable_collider);
                    _already_collided.insert(CollisionPair(movable_collider->behaviour_id(), to_collide->behaviour_id()));
                }
            }
            iter++;
        } else {
            auto to_erase = iter;
            iter++;
            _movable_colliders.erase(to_erase);
        }
    }
}

std::vector<std::weak_ptr<BaseCollider>> CollisionEngine::GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->GetPotentiallyCollidingWith(collider);
    } else {
        std::vector<std::weak_ptr<BaseCollider>> result;
        for (auto iter = _static_colliders.begin(); iter != _static_colliders.end(); ) {
            if (auto static_collider = iter->lock()) {
                result.push_back(static_collider);
                iter++;
            } else {
                auto to_erase = iter;
                iter++;
                _static_colliders.erase(to_erase);
            }
        }
        for (auto iter = _movable_colliders.begin(); iter != _movable_colliders.end(); ) {
            if (auto movable_collider = iter->lock()) {
                if (collider->behaviour_id() != movable_collider->behaviour_id()) {
                    result.push_back(movable_collider);
                }
                iter++;
            } else {
                auto to_erase = iter;
                iter++;
                _movable_colliders.erase(to_erase);
            }
        }
        return result;
    }
}

void CollisionEngine::onGameObjectAdded(const std::shared_ptr<GameObject>& shared_obj) {
    for (auto& base_collider : shared_obj->getBehaviours<BaseCollider>()) {
        auto shared_collider = base_collider.lock();
        shared_collider->_is_collision_engine_attached = true;
        if (shared_collider->IsStatic()) {
            _static_colliders.push_back(shared_collider);
        } else {
            _movable_colliders.push_back(shared_collider);
        }
        if (_spatial_partitioner) {
            _spatial_partitioner->PartitionCollider(shared_collider);
        }
    }
}

void CollisionEngine::UpdateSpatialPartition(const std::weak_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->UpdatePartitionedCollider(collider);
    }
}

void CollisionEngine::UnregisterCollider(const std::weak_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        _spatial_partitioner->RemovePartitionedCollider(collider);
    }
}
