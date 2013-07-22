#ifndef MATH_USER_H
#define MATH_USER_H

#include <functional>

namespace user {

bool isLevelGroupLocked(const int group);
void markLevelGroupUnlocked(const int group);

int getLevelStars(const int group, const int level);
void setLevelStars(const int group, const int level, const int stars);

void addStarChangeCallback(std::function<void (const int group, const int level)> callback);

} // namespace user

#endif // MATH_USER_H