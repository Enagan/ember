// File containing the private API and internal workings of Collider to avoid polluting the public API file.

private:
    void CastCollisionEvent(const std::weak_ptr<BaseCollider>& collided_with) override {
        game_object().template CastEvent<Collision<ColliderSubType>>(Collision<ColliderSubType>(collided_with));
    }
    
    std::weak_ptr<BaseCollider> weak_from_this() override {
        return std::static_pointer_cast<BaseCollider>(game_object().template getBehaviour<ColliderSubType>().lock());
    }
