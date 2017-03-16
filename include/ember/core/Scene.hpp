#ifndef Scene_hpp
#define Scene_hpp

#include <unordered_map>
#include <memory>
#include <initializer_list>

#include "GameObject.hpp"
#include "BaseSystem.hpp"
#include <typeindex>

namespace ember {
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
public:
    inline bool hasStarted() {return _hasStarted;}

	GameObject& addGameObject();
    bool hasGameObject(GameObject::id index) const;
    GameObject& getGameObject(GameObject::id index);
    void removeGameObject(GameObject::id index);

	template <typename SystemSubType, typename... Args>
    void attachSystem(Args&&... args);
    template <typename SystemSubType>
	bool hasSystem();

    /// Triggers an event through all the child components that are subclasses of ListenTo<EventType>
    template <typename EventType>
	void BroadcastEvent(const EventType& event);

private:
    void FilterGameObjectThroughAllSystems(const std::shared_ptr<GameObject>& object);
    void FilterAllGameObjectsThroughSystem(const std::shared_ptr<BaseSystem>& system);

    void Swap(Scene&& other);
private:
    GameObject::id _next_game_object_index = 0;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> _objects_in_scene;
	bool _hasStarted{ false };

	std::unordered_map<std::type_index, std::shared_ptr<BaseSystem>> _systems_in_scene;
};
}
#include "_impl/Scene_impl.hpp"

#endif
