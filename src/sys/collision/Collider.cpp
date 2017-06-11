#include "ember/sys/collision/Collider.hpp"
#include "ember/core/GameObject.hpp"
#include "ember/core/Scene.hpp"
#include "ember/sys/collision/CollisionEngine.hpp"

using namespace ember::sys::collision;

BaseCollider::BaseCollider(bool is_static) : _is_static(is_static) {};

void BaseCollider::onEnd() {
    if(_is_collision_engine_attached && game_object().scene().hasSystem<CollisionEngine>()) {
        game_object().scene().refSystem<CollisionEngine>().UnregisterCollider(weak_from_this());
    }
}
