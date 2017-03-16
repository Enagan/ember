#include "ember/system/collision/SpatialPartition.hpp"
#include "ember/system/collision/CollisionEngine.hpp"
#include "ember/core/Scene.hpp"

ember::Scene& ember::system::collision::SpatialPartition::scene() { return _owning_engine->scene(); }
