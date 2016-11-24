#ifndef Ember_CollisionEvent_hpp
#define Ember_CollisionEvent_hpp

#include "Collision.hpp"

namespace ember {
namespace collision {
    
template <typename MyCollider, typename OtherCollider = class BaseCollider>
class HandlesCollision : public addons::ListensTo<Collision<MyCollider>> {
public:
    void Handle(const Collision<MyCollider>& message) override {
        if (auto cast_collider = std::dynamic_pointer_cast<OtherCollider>(message.collided_with().lock())) {
            HandleCollisionWith(cast_collider);
        }
    }
    virtual void HandleCollisionWith(const std::shared_ptr<OtherCollider>& collided_with) = 0;
};

}}
#endif