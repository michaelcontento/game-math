#ifndef MATH_USER_H
#define MATH_USER_H

#include <functional>

namespace user {

bool isLevelGroupLocked(const int group);
void setLevelGroupLocked(const int group, const bool flag = true);
bool allLevelGroupsUnlocked();

int getLevelStars(const int group, const int level);
void setLevelStars(const int group, const int level, const int stars);

void addStarChangeCallback(std::function<void (const int group, const int level)> callback);

bool hasAdsEnabled();
void setAdsEnabled(const bool flag = true);

bool hasSoundEnabled();
void setSoundEnabled(const bool flag = true);

bool hasMusicEnabled();
void setMusicEnabled(const bool flag = true);

} // namespace user

#endif // MATH_USER_H