#ifndef Ember_Behaviour_hpp
#define Ember_Behaviour_hpp

#include <memory>
#include <cstddef>

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
#endif
