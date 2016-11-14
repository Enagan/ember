/// Implementation of template methods for Scene

template <typename EventType>
void Scene::BroadcastEvent(const EventType& event) {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> traversal_copy(_objectsInScene);
    for (auto& game_object : traversal_copy) {
        game_object.second->CastEvent<EventType>(event);
    }
}