#ifndef Ember_Collider_hpp
#define Ember_Collider_hpp

#include "ember/core/Behaviour.hpp"
#include "ember/core/GameObject.hpp"
#include "Collision.hpp"

namespace ember {
namespace collision {
    
class BaseCollider : public Behaviour {
public:
    BaseCollider(bool is_static);
    
    virtual void onStart() override;
	virtual void onPostUpdate() override;
    virtual void onEnd() override;
    
public:
    virtual bool CollidesWith(const std::shared_ptr<BaseCollider>& other) = 0;
    inline bool IsStatic() const { return _is_static; }
    
    virtual std::weak_ptr<BaseCollider> weak_from_this() = 0;
    
    virtual void CastCollisionEvent(const std::weak_ptr<BaseCollider>& collided_with) = 0;
    
private:
    bool _is_static;
};

template<typename ColliderSubType>
class Collider : public BaseCollider {
public:
    Collider(bool is_static) : BaseCollider(is_static) {}
    void CastCollisionEvent(const std::weak_ptr<BaseCollider>& collided_with) override {
        game_object().template CastEvent<Collision<ColliderSubType>>(Collision<ColliderSubType>(collided_with));
    }
    std::weak_ptr<BaseCollider> weak_from_this() override {
        return std::static_pointer_cast<BaseCollider>(game_object().template getBehaviour<ColliderSubType>().lock());
    }
};

}
}
#endif