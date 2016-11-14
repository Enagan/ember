#ifndef Ember_ListensTo_hpp
#define Ember_ListensTo_hpp

namespace ember {
namespace addons {
    
template <typename TypeToReceive>
class ListensTo {
public:
    virtual void Handle(const TypeToReceive& message) = 0;
};

}}
#endif