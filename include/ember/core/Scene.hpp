#ifndef Scene_hpp
#define Scene_hpp

#include <unordered_map>
#include <memory>
#include <initializer_list>

#include "GameObject.hpp"

namespace ember {
namespace collision {
    class CollisionEngine;
}
class Scene {
public:
	Scene();
    ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other);
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other);
public:
	void onStart();
    void onPreUpdate();
    void onUpdate(double deltaT);
    void onPostUpdate();
    void onCollision();
    void onPostCollision();
public:
    inline bool hasStarted() {return _hasStarted;}

	GameObject& addGameObject();
    bool hasGameObject(GameObject::id index) const;
    GameObject& getGameObject(GameObject::id index);
    void removeGameObject(GameObject::id index);

    /// Triggers an event through all the child components that are subclasses of ListenTo<EventType>
    template <typename EventType>
	void BroadcastEvent(const EventType& event);

    collision::CollisionEngine& CollisionEngine();
private:
    GameObject::id _next_game_object_index = 0;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> _objectsInScene;
	bool _hasStarted{ false };

    std::unique_ptr<collision::CollisionEngine> _collision_engine;
};
}
#include "_impl/Scene_impl.hpp"

#endif
