#include <set>
#include <algorithm>
#include "ember/sys/collision/CollisionEngine.hpp"
#include "ember/core/Scene.hpp"

using namespace ember::sys::collision;

void CollisionEngine::onUpdate(double /*deltaT*/) {
    /// Update partition info of movable colliders
    if (_spatial_partitioner) {
        for (const auto& movable_weak : _movable_colliders) {
            _spatial_partitioner->UpdatePartitionedCollider(movable_weak);
        }
    }

    using CollisionPair = std::pair<Behaviour::id, Behaviour::id>;
    std::set<CollisionPair> _already_collided;
    auto traversal_copy = _movable_colliders;
    for (const auto& iter : traversal_copy) {
        if (auto movable_collider = iter.lock()) {
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
}

std::vector<std::weak_ptr<BaseCollider>> CollisionEngine::GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider) {
    if (_spatial_partitioner) {
        return _spatial_partitioner->GetPotentiallyCollidingWith(collider);
    } else {
        std::vector<std::weak_ptr<BaseCollider>> result;
        result.reserve( _static_colliders.size() + _movable_colliders.size() ); // preallocate memory
        result.insert( result.end(), _static_colliders.begin(), _static_colliders.end() );
        result.insert( result.end(), _movable_colliders.begin(), _movable_colliders.end() );
        return result;
    }
}

void CollisionEngine::onGameObjectAdded(const std::weak_ptr<GameObject>& weak_obj) {
    if (auto shared_obj = weak_obj.lock()) {
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
}

void CollisionEngine::UnregisterCollider(const std::weak_ptr<BaseCollider>& collider) {
    if (auto shared_collider = collider.lock()) {
        auto removal_fun = [&shared_collider](const std::weak_ptr<BaseCollider>& collider) {
            if (auto shared_traversal = collider.lock()) {
                return shared_collider->behaviour_id() == shared_traversal->behaviour_id();
            } else {
                return true;
            }
        };
        if (shared_collider->IsStatic()) {
            auto end_iter = std::remove_if(_static_colliders.begin(), _static_colliders.end(), removal_fun);
            _static_colliders.erase(end_iter, _static_colliders.end());
        } else {
            auto end_iter = std::remove_if(_movable_colliders.begin(), _movable_colliders.end(), removal_fun);
            _movable_colliders.erase(end_iter, _movable_colliders.end());
        }
    }

    if (_spatial_partitioner) {
        _spatial_partitioner->RemovePartitionedCollider(collider);
    }
}
