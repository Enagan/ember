#ifndef Ember_Behaviour_hpp
#define Ember_Behaviour_hpp

#include <memory>
#include <cstddef>
#include <stdexcept>

namespace ember {

class GameObject;
class Scene;
class Behaviour {
	friend GameObject;
public:
    using id = std::pair<std::size_t, std::size_t>;
protected:
    Behaviour() = default;
    Behaviour(const Behaviour& other) = delete;
    Behaviour& operator=(const Behaviour& other) = delete;
public:
	// Base
	inline GameObject& game_object() { return *_gameObjectOwner; };
    inline const GameObject& game_object() const { return *_gameObjectOwner; };
    bool is_attached() const { return _gameObjectOwner != nullptr; };
    inline Behaviour::id behaviour_id() const { return _id; }

    template <typename BehaviourSubType>
	bool hasSibling();
    template <typename BehaviourSubType>
	BehaviourSubType& refSibling() throw(std::invalid_argument);
    template <typename BehaviourSubType>
	const BehaviourSubType& refSibling() const throw(std::invalid_argument);
    template <typename BehaviourSubType>
	std::weak_ptr<BehaviourSubType> getSibling();

public:
	// Overrideable
    virtual void onStart(){};
    virtual void onPreUpdate(){};
	virtual void onUpdate(double /*deltaT*/){};
	virtual void onPostUpdate(){};
    virtual void onPostCollision(){};
    virtual void onEnd(){};
private:
	void setGameObjectOwner(GameObject* gameObjectOwner) { _gameObjectOwner = gameObjectOwner; }
private:
    Behaviour::id _id = Behaviour::id{0, 0};

    // Weak pointer to owning GameObject instance
	GameObject* _gameObjectOwner = nullptr;
};
}
#include "_impl/Behaviour_impl.hpp"

#endif
