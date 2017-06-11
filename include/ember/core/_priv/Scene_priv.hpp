// File containing the private API and internal workings of a Scene, to avoid polluting the public API file.

// Class private methods
private:
    void onStart();
    void onPreUpdate();
    void onUpdate(double deltaT);
    void onPostUpdate();

    void FilterGameObjectThroughAllSystems(const std::shared_ptr<GameObject>& object);
    void FilterAllGameObjectsThroughSystem(const std::shared_ptr<BaseSystem>& system);

    void Swap(Scene&& other);

// Class Variables
private:
    GameObject::id _next_game_object_index = 0;
    bool _hasStarted{ false };

    std::map<GameObject::id, std::shared_ptr<GameObject>> _objects_in_scene;
	std::map<std::type_index, std::shared_ptr<BaseSystem>> _systems_in_scene;
