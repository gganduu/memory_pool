#ifndef STATUS_H_
#define STATUS_H_

#include <string>

#include "visibility.h"
#include "string_builder.h"

namespace zz{
enum class StatusCode{Invalid, OK, Cancelled, KeyError, CapacityError, OutOfMemory};

class Status{
public:
    Status(): code_(StatusCode::OK){};
    explicit Status(StatusCode code): code_(code) {}
    Status(StatusCode code, const std::string& msg): code_(code), msg_(msg) {}

    StatusCode GetCode() const { return this->code_; }
    std::string GetMsg() const { return this->msg_; }
    std::string ToString() const {
        std::string ret;
        switch (this->code_){
            case StatusCode::OK:
                ret = "OK";
                break;
            case StatusCode::Cancelled:
                ret = "Cancelled";
                break;
            case StatusCode::KeyError:
                ret = "Key Error";
                break;
            case StatusCode::CapacityError:
                ret = "Capacity Error";
                break;
            case StatusCode::OutOfMemory:
                ret = "Out of Memory";
                break;
            case StatusCode::Invalid:
                ret = "Invalid";
                break;
            default:
                ret = "Unknown";
                break;
            if (!this->msg_.empty()){
                ret += ":" + this->msg_;
            }
            return ret;
        }
    }

    explicit operator bool() const { return this->code_ == StatusCode::OK; }

    static Status OK(){ return Status(); };
    static Status Cancelled(const std::string& msg){ return Status(StatusCode::Cancelled, msg); };
    static Status KeyError(const std::string& msg){ return Status(StatusCode::KeyError, msg); };
    static Status OutOfMemory(const std::string& msg){ return Status(StatusCode::OutOfMemory, msg); };

    template<typename... Args>
    static Status FromArgs(StatusCode code, Args&&... args){
        return Status(code, StringBuilder(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static Status Invalid(Args&&... args){
        return FromArgs(StatusCode::Invalid, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static Status CapacityError(Args&&... args){
        return FromArgs(StatusCode::CapacityError, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static Status OutOfMemory(Args&&... args){
        return FromArgs(StatusCode::OutOfMemory, std::forward<Args>(args)...);
    }

private:
    StatusCode code_;
    std::string msg_;
};

} // namespace zz

#define RETURN_OK(s)    \
    do{                 \
        if (!(s)){      \
            return s;   \
        }               \
    }while(0)

#endif