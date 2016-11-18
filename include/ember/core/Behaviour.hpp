#ifndef Ember_Behaviour_hpp
#define Ember_Behaviour_hpp

#include <memory>

namespace ember {

class GameObject;
class Scene;
class Behaviour {
	friend GameObject;
protected:
    Behaviour() = default;
    Behaviour(const Behaviour& other) = delete;
    Behaviour& operator=(const Behaviour& other) = delete;
public:
	// Base
	inline GameObject& game_object() { return *_gameObjectOwner; };
    inline const GameObject& cgame_object() const { return *_gameObjectOwner; };
    bool is_attached() const { return _gameObjectOwner != nullptr; };
public:
	// Overrideable
    virtual void onStart(){};
	virtual void onUpdate(double /*deltaT*/){};
	virtual void onPostUpdate(){};
    virtual void onEnd(){};
private:
	void setGameObjectOwner(GameObject* gameObjectOwner) { _gameObjectOwner = gameObjectOwner; }
private:
    // Weak pointer to owning GameObject instance
	GameObject* _gameObjectOwner = nullptr;
};

}
#endif