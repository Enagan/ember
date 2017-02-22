#include "ember/core/Scene.hpp"
#include "ember/collision/CollisionEngine.hpp"

using namespace ember;

Scene::Scene() : _collision_engine(new collision::CollisionEngine()) {
    _collision_engine->_owning_scene = this;
};

Scene::~Scene() {};

Scene::Scene(Scene&& other) : _next_game_object_index(other._next_game_object_index), _hasStarted(other._hasStarted) {
	_objectsInScene.swap(other._objectsInScene);
    _collision_engine.swap(other._collision_engine);
    _collision_engine->_owning_scene = this;
}

Scene& Scene::operator=(Scene&& other) {
    _next_game_object_index = other._next_game_object_index;
	_hasStarted = other._hasStarted;
	_objectsInScene.swap(other._objectsInScene);
    _collision_engine.swap(other._collision_engine);
    _collision_engine->_owning_scene = this;
	return *this;
}

void Scene::onStart() {
    _collision_engine->_owning_scene = this;
	_hasStarted = true;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
	for (auto& gameObject : delayed_deletion_copy) {
		gameObject.second->onStart();
	}
}

void Scene::onPreUpdate() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
	for (auto& gameObject : delayed_deletion_copy) {
		gameObject.second->onPreUpdate();
	}
}

void Scene::onUpdate(double deltaT) {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
	for (auto& gameObject : delayed_deletion_copy) {
		gameObject.second->onUpdate(deltaT);
	}
}

void Scene::onPostUpdate() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
	for (auto& gameObject : delayed_deletion_copy) {
		gameObject.second->onPostUpdate();
	}
}

void Scene::onCollision() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
    _collision_engine->TriggerCollisions();
}

void Scene::onPostCollision() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objectsInScene);
	for (auto& gameObject : delayed_deletion_copy) {
		gameObject.second->onPostCollision();
	}
}

GameObject& Scene::addGameObject() {
    auto id_for_gameobject = _next_game_object_index++;
    _objectsInScene[id_for_gameobject] = std::make_shared<GameObject>();
    auto& new_object = _objectsInScene[id_for_gameobject];
    new_object->_id = id_for_gameobject;
    new_object->_parent_scene = this;
	if (_hasStarted) {
		new_object->onStart();
	}
	return *new_object;
}

bool Scene::hasGameObject(GameObject::id index) const {
    return _objectsInScene.count(index) == 1;
}

GameObject& Scene::getGameObject(GameObject::id index) {
    return *_objectsInScene.at(index);
}

void Scene::removeGameObject(GameObject::id index) {
    _objectsInScene.erase(index);
}

collision::CollisionEngine& Scene::CollisionEngine() {
    return *_collision_engine;
}
