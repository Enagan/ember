// Class private methods
private:
    void FilterGameObject(const std::shared_ptr<GameObject>& shared_obj);

// Class Variables
private:
    // Weak pointer to the scene the gameobject is attached to. If the system exists
    // the scene WILL exist.
    class ember::Scene* _parent_scene = nullptr;
    SystemFilter _filter_fun;
