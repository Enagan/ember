#include "ember/core/Scene.hpp"

using namespace ember;

Scene::Scene(Scene&& other) : _next_game_object_index(other._next_game_object_index), _hasStarted(other._hasStarted) {
	_objectsInScene.swap(other._objectsInScene);
}

Scene& Scene::operator=(Scene&& other) {
    _next_game_object_index = other._next_game_object_index;
	_hasStarted = other._hasStarted;
	_objectsInScene.swap(other._objectsInScene);
	return *this;
}

void Scene::onStart() {
	_hasStarted = true;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> traversal_copy(_objectsInScene);
	for (auto& gameObject : traversal_copy) {
		gameObject.second->onStart();
	}
}

void Scene::onUpdate(double deltaT) {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> traversal_copy(_objectsInScene);
	for (auto& gameObject : traversal_copy) {
		gameObject.second->onUpdate(deltaT);
	}
}

void Scene::onPostUpdate() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> traversal_copy(_objectsInScene);
	for (auto& gameObject : traversal_copy) {
		gameObject.second->onPostUpdate();
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