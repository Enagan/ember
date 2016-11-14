#ifndef Scene_hpp
#define Scene_hpp

#include <unordered_map>
#include <memory>
#include <initializer_list>

#include "GameObject.hpp"

namespace ember {

class Scene {
public:
	Scene() = default;
	Scene(const Scene& other) = delete;
	Scene(Scene&& other);
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other);
public:
	virtual void onStart();
    virtual void onUpdate(double deltaT);
    virtual void onPostUpdate();
public:
	virtual GameObject& addGameObject();
    bool hasGameObject(GameObject::id index) const;
    GameObject& getGameObject(GameObject::id index);
    void removeGameObject(GameObject::id index);
    
    /// Triggers an event through all the child components that are subclasses of ListenTo<EventType>
    template <typename EventType>
	void BroadcastEvent(const EventType& event);
private:
    GameObject::id _next_game_object_index = 0;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> _objectsInScene;
	bool _hasStarted{ false };
};
#include "_impl/Scene_impl.hpp"

}
#endif