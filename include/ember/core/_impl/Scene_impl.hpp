/// Implementation of template methods for Scene
namespace ember {

template <typename SystemSubType, typename... Args>
void Scene::attachSystem(Args&&... args) {
    auto type_index = std::type_index(typeid(SystemSubType));
    auto ret = _systems_in_scene.emplace(std::pair<std::type_index, std::shared_ptr<BaseSystem>>{type_index, std::make_shared<SystemSubType>(std::forward<Args>(args)...)});
    if (ret.second) {
        (*ret.first).second->_parent_scene = this;
        FilterAllGameObjectsThroughSystem((*ret.first).second);
    	if (_hasStarted) {
    		(*ret.first).second->onStart();
    	}
    }
}

template <typename SystemSubType>
bool Scene::hasSystem() {
    return _systems_in_scene.count(std::type_index(typeid(SystemSubType))) != 0;
}

template <typename EventType>
void Scene::BroadcastEvent(const EventType& event) {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> traversal_copy(_objectsInScene);
    for (auto& game_object : traversal_copy) {
        game_object.second->CastEvent<EventType>(event);
    }
    for (auto& system_in_scene : _systems_in_scene) {
        if(auto cast_system = std::dynamic_pointer_cast<addons::ListensTo<EventType>>(system_in_scene.second))
        {
            cast_system->Handle(event);
        }
    }
}

}
