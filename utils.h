#ifndef UTILS_H_
#define UTILS_H_

#include <optional>
#include <string>

#include "visibility.h"
#include "status.h"

namespace zz{
VISIBLE_EXPORT
std::optional<std::string> GetEnv (const std::string& name);
VISIBLE_EXPORT
Status SetEnv(const char* name, const char* value);
VISIBLE_EXPORT
Status SetEnv(const std::string& name, const std::string& value);

} // namespace zz

#endif