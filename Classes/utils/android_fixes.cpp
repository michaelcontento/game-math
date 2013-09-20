#include "android_fixes.h"

#if AVALON_PLATFORM_IS_ANDROID

namespace std {

string to_string(int val)
{
    std::ostringstream os;
    os << val;
    return os.str();
}

int stoi(const std::string& str)
{
    std::istringstream is(str);
    int t;
    is >> t;
    return t;
}

} // namespace std

#endif /* AVALON_PLATFORM_IS_ANDROID */