
// Class private methods
private:
	inline void setGameObjectOwner(GameObject* gameObjectOwner) { _gameObjectOwner = gameObjectOwner; }

// Class Variables
private:
    Behaviour::id _id = Behaviour::id{0, 0};
    // Weak pointer to owning GameObject instance
	GameObject* _gameObjectOwner = nullptr;
