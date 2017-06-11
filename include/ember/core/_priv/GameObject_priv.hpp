// File containing the private API and internal workings of a GameObject to avoid polluting the public API file of the Gameobject

// Class private methods
private:
    void CheckForCacheInvalidation();

    void onStart();
    void onPreUpdate();
    void onUpdate(double deltaT);
    void onPostUpdate();
    void onEnd();

// Class Variables
private:
    GameObject::id _id = 0;
    bool _hasStarted{ false };
    bool _hasEnded{ false };
    std::size_t _next_behaviour_index = 0;
    std::map<std::type_index, std::shared_ptr<Behaviour>> _behaviours;
    mutable std::map<std::type_index, std::vector<std::weak_ptr<Behaviour>>> _get_behaviours_cache;
    bool _behaviours_changed = false;

    struct SerializedCacheBase;
    template <typename SerializableType> struct SerializedCacheSub;
    std::map<std::type_index, std::unique_ptr<SerializedCacheBase>> _serialization_cache;
    std::map<std::type_index, std::unique_ptr<SerializedCacheBase>> _partial_serialization_cache;

    // Weak pointer to the scene the gameobject is attached to. If the gameobject exists
    // The scene WILL exist
    class Scene* _parent_scene = nullptr;
