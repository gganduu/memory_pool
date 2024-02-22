#include <optional>
#include <cstring>

#include "../include/utils.h"

namespace zz{
std::optional<std::string> GetEnv(const char* name){
    char* c_str = getenv(name);
    if (c_str == nullptr){
        return {};
    } else {
        return std::string(c_str);
    }
}

Status SetEnv(const char* name, const char* value){
    if (setenv(name, value, 1) == 0){
        return Status::OK();
    } else {
        return Status::Invalid("Failed to set environment variable");
    }
}

Status SetEnv(const std::string& name, const std::string& value){
    return SetEnv(name.c_str(), value.c_str());
}

} // namespace zz