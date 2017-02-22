#ifndef Ember_Serializable_hpp
#define Ember_Serializable_hpp

namespace ember {
namespace addons {

template <typename TypeToSerializeTo>
class SerializableInto {
public:
    virtual void SerializeInto(TypeToSerializeTo& /*into*/){};
    virtual bool PartialSerializeInto(TypeToSerializeTo& /*into*/){ return false; };
};

}}
#endif
