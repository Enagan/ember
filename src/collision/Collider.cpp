#include "ember/system/collision/Collider.hpp"
#include "ember/core/GameObject.hpp"
#include "ember/core/Scene.hpp"
#include "ember/system/collision/CollisionEngine.hpp"

using namespace ember::system::collision;

BaseCollider::BaseCollider(bool is_static) : _is_static(is_static) {};

void BaseCollider::onPostUpdate() {
    if(_is_collision_engine_attached && !_is_static && game_object().scene().hasSystem<CollisionEngine>()) {
        game_object().scene().refSystem<CollisionEngine>().UpdateSpatialPartition(weak_from_this());
    }
}

void BaseCollider::onEnd() {
    if(_is_collision_engine_attached && game_object().scene().hasSystem<CollisionEngine>()) {
        game_object().scene().refSystem<CollisionEngine>().UnregisterCollider(weak_from_this());
    }
}
