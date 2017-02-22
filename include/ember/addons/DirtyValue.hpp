#ifndef Stagecraft_DirtyValue_hpp
#define Stagecraft_DirtyValue_hpp

namespace stagecraft {
namespace addons {
    
template <typename ParamType>
class DirtyValue {
public:
    DirtyValue(ParamType data) : _data(std::move(data)), _is_dirty(true) {} 

    const ParamType& get() const { return _data; }
    void set(ParamType new_data) { std::swap(_data, new_data); _is_dirty = true; }
    
    bool is_dirty() const { return _is_dirty; }
    void clean() { _is_dirty = false; }
    
private:
    bool _is_dirty;
    ParamType _data;
};

}}
#endif