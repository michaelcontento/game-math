#include "user.h"

#include <string>
#include <vector>
#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace avalon;

namespace user {

namespace impl {

std::string getLockedKey(const int group)
{
    return std::to_string(group).append("_locked");
}

std::string getStarsKey(const int group, const int level)
{
    return std::to_string((group * 100) + level).append("_stars");
}

static std::vector<std::function<void (const int group, const int level)>> starCallbacks = {};

} // namespace impl

bool isLevelGroupLocked(const int group)
{
    if (group == 0) {
        return false;
    }

    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey(impl::getLockedKey(group).c_str(), true);
}

void setLevelGroupLocked(const int group, const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey(impl::getLockedKey(group).c_str(), flag);
    settings->flush();
}

bool allLevelGroupsUnlocked()
{
    for (int i = 1; i <= 4; ++i) {
        if (isLevelGroupLocked(i)) {
            return false;
        }
    }

    return true;
}

int getLevelStars(const int group, const int level)
{
    if (group <= 0 || level <= 0) {
        return true;
    }

    auto settings = UserDefault::getInstance();
    return settings->getIntegerForKey(impl::getStarsKey(group, level).c_str(), 0);
}

void setLevelStars(const int group, const int level, const int stars)
{
    if (getLevelStars(group, level) >= stars) {
        return;
    }

    auto settings = UserDefault::getInstance();
    settings->setIntegerForKey(impl::getStarsKey(group, level).c_str(), stars);
    settings->flush();

    for (auto& callback : impl::starCallbacks) {
        callback(group, level);
    }
}

void addStarChangeCallback(std::function<void (const int group, const int level)> callback)
{
    impl::starCallbacks.push_back(callback);
}

bool hasAdsEnabled()
{
    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey("ads", true);
}

void setAdsEnabled(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("ads", flag);
    settings->flush();

    avalon::ads::Manager::enabled = flag;
    avalon::ads::Manager::hide();
}

bool hasSoundEnabled()
{
    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey("sounds", true);
}

void setSoundEnabled(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("sounds", flag);
    settings->flush();
}

} // namespace user