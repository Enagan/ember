class BaseCollider : public Behaviour {
    friend class CollisionEngine;
public:
    BaseCollider(bool is_static);
    virtual void onEnd() override;

public:
    virtual bool CollidesWith(const std::shared_ptr<BaseCollider>& other) = 0;
    inline bool IsStatic() const { return _is_static; }

    virtual std::weak_ptr<BaseCollider> weak_from_this() = 0;

    virtual void CastCollisionEvent(const std::weak_ptr<BaseCollider>& collided_with) = 0;

private:
    bool _is_static;
    bool _is_collision_engine_attached = false;
};
