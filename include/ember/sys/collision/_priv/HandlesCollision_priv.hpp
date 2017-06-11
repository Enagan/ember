// File containing the private API and internal workings of HandlesCollision to avoid polluting the public API file.

void Handle(const Collision<MyCollider>& message) override final {
    if (auto cast_collider = std::dynamic_pointer_cast<OtherCollider>(message.collided_with().lock())) {
        HandleCollisionWith(cast_collider);
    }
}
