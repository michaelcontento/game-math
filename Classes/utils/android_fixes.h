#ifndef MATH_ANDROIDFIXES_H
#define MATH_ANDROIDFIXES_H

#include <avalon/utils/platform.h>

#if AVALON_PLATFORM_IS_ANDROID

namespace std {

template <typename T>
string to_string(const T &t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

int stoi(const std::string &str)
{
    std::istringstream is(str);
    int t;
    is >> t;
    return t;
}

} // namespace std

#endif /* AVALON_PLATFORM_IS_ANDROID */
#endif /* MATH_ANDROIDFIXES_H */
