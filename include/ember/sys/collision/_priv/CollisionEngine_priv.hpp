// File containing the private API and internal workings of CollisionEngine to avoid polluting the public API file.

// Class private methods
// From System
protected:
    void onPreUpdate() override {} // No work to be done on PreUpdate
    void onUpdate(double /*deltaT*/) override;
    void onPostUpdate() override {} // No work to be done onUpdate
    void onGameObjectAdded(const std::weak_ptr<GameObject>& weak_obj) override;
    void onGameObjectRemoved(const std::weak_ptr<GameObject>&) override {}

private:
    std::vector<std::weak_ptr<BaseCollider>> GetCollisionShortlistForCollider(const std::shared_ptr<BaseCollider>& collider);
    void UnregisterCollider(const std::weak_ptr<BaseCollider>& collider);

// Class Variables
private:
    std::vector<std::weak_ptr<BaseCollider>> _static_colliders;
    std::vector<std::weak_ptr<BaseCollider>> _movable_colliders;
    std::unique_ptr<SpatialPartition> _spatial_partitioner;
