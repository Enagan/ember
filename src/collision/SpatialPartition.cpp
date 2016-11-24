#include "ember/collision/SpatialPartition.hpp"
#include "ember/collision/CollisionEngine.hpp"
#include "ember/core/Scene.hpp"

ember::Scene& ember::collision::SpatialPartition::scene() { return _owning_engine->scene(); }
