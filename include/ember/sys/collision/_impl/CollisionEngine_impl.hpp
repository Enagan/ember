namespace ember {
namespace sys {
namespace collision {

template <typename SpatialPartitionSubType, typename... Args>
void CollisionEngine::withSpatialPartitioner(Args&&... args) {
    _spatial_partitioner = std::unique_ptr<SpatialPartition>(new SpatialPartitionSubType(std::forward<Args>(args)...));
    _spatial_partitioner->_owning_engine = this;
}

template <typename SpatialPartitionSubType>
void CollisionEngine::hasSpatialPartitionerOfType() {
    return dynamic_cast<SpatialPartitionSubType*>(_spatial_partitioner.get()) != nullptr;
}

template <typename SpatialPartitionSubType>
SpatialPartitionSubType& CollisionEngine::refSpatialPartitionerOfType() throw(std::invalid_argument) {
    if (auto cast_spatial_partitioner = dynamic_cast<SpatialPartitionSubType*>(_spatial_partitioner.get())) {
        return *(cast_spatial_partitioner);
    } else {
        throw std::invalid_argument("CollisionEngine::refSpatialPartitionerOfType - Engine has no partitioner of type " +
            std::string(typeid(SpatialPartitionSubType).name()));
    }
}

}
}
}
