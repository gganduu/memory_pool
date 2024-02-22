#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <ostream>
#include <sstream>
#include <memory>

namespace zz{

class StringStreamWrapper{
public:
    StringStreamWrapper(): ss_(std::make_unique<std::ostringstream>()), os_(*ss_){}
    std::ostream& GetStream() const { return this->os_; }
    std::string ToString() const {
        return this->ss_->str();
    }
private:
    std::ostream& os_;
    std::unique_ptr<std::ostringstream> ss_;
};

template <typename Head, typename... Tail>
void StringBuilderRecursive(std::ostream& os, Head&& head, Tail&&... tail){
    os << head;
    if constexpr (sizeof...(tail) > 0){
        StringBuilderRecursive(os, std::forward<Tail>(tail)...);
    }
}

template<typename... Args>
std::string StringBuilder(Args&&... args){
    StringStreamWrapper stream_wrapper;
    StringBuilderRecursive(stream_wrapper.GetStream(), std::forward<Args>(args)...);
    return stream_wrapper.ToString();
}

} // namespace zz

#endif