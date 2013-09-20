#ifndef MATH_ANDROIDFIXES_H
#define MATH_ANDROIDFIXES_H

#include <avalon/utils/platform.h>

#if AVALON_PLATFORM_IS_ANDROID

namespace std {

string to_string(int val);
int stoi(const std::string &str);

} // namespace std

#endif /* AVALON_PLATFORM_IS_ANDROID */
#endif /* MATH_ANDROIDFIXES_H */
