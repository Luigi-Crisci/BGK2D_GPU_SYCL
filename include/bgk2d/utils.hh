#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <string_view>
#include <vector>

namespace bgk::utils {
template<typename T>
static constexpr auto type_name() {
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto type_name() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl type_name<";
    suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}


static inline void trim(std::string &s) {
    std::erase_if(s, [](char c) { return std::isspace(c); });
}

} // namespace bgk::utils

#endif // __UTILS_H__