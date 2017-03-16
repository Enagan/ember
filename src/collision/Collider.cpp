#include "ember/system/collision/Collider.hpp"
#include "ember/core/GameObject.hpp"
#include "ember/core/Scene.hpp"
#include "ember/system/collision/CollisionEngine.hpp"

using namespace ember::system::collision;

BaseCollider::BaseCollider(bool is_static) : _is_static(is_static) {};

void BaseCollider::onStart() {
    //game_object().scene().CollisionEngine().RegisterCollider(weak_from_this());
}

void BaseCollider::onPostUpdate() {
    /*if (!_is_static) {
        game_object().scene().CollisionEngine().UpdateSpatialPartition(weak_from_this());
    }*/
}

void BaseCollider::onEnd() {
    //game_object().scene().CollisionEngine().UnregisterCollider(weak_from_this());
}
