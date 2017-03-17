#ifndef Ember_Collision_hpp
#define Ember_Collision_hpp

namespace ember {
namespace system {
namespace collision {

template <typename MyCollider>
class Collision {
public:
    Collision(const std::weak_ptr<class BaseCollider>& collided_with) : _collided_with(collided_with) {}
    inline std::weak_ptr<class BaseCollider> collided_with() const { return _collided_with; }
private:
    std::weak_ptr<class BaseCollider> _collided_with;
};

}
}
}
#endif
