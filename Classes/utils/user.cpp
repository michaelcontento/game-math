#include "user.h"

#include <string>
#include <vector>
#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
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
    return false;
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
    return 3;
    if (group <= 0 || level <= 0) {
        return 0;
    }

    auto settings = UserDefault::getInstance();
    return settings->getIntegerForKey(impl::getStarsKey(group, level).c_str(), 0);
}

void setLevelStars(const int group, const int level, const int stars)
{
    auto oldStars = getLevelStars(group, level);
    if (oldStars >= stars) {
        return;
    }

    // store local
    auto settings = UserDefault::getInstance();
    settings->setIntegerForKey(impl::getStarsKey(group, level).c_str(), stars);
    settings->flush();

    // update gamecenter
    auto gc = GameCenter();

    auto keyComplete = std::string("com.coragames.math.ac.") + std::to_string(group);
    auto valueComplete = 100.0 / 16.0 * getSolvedLevelsInGroup(group);
    gc.postAchievement(keyComplete.c_str(),  valueComplete);

    auto keyStars = keyComplete + ".stars";
    auto valueStars = 100.0 / (16.0 * 3) * getStarsInGroup(group);
    gc.postAchievement(keyStars.c_str(),  valueStars);

    auto totalStars = getStarsInAllGroups();
    gc.postScore("com.coragames.math.lb.allstars", totalStars);

    auto valueAllStars = 100.0 / (16.0 * 3 * 10) * totalStars;
    gc.postAchievement("com.coragames.math.ac.allstars",  valueAllStars);

    if (oldStars == 0) {
        auto totalLevels = getSolvedLevelsInAllGroups();
        gc.postScore("com.coragames.math.lb.alllevels", totalLevels);
    }

    // trigger callbacks
    for (auto& callback : impl::starCallbacks) {
        callback(group, level);
    }
}

int getSolvedLevelsInAllGroups()
{
    int result = 0;
    for (int i = 1; i <= 10; ++i) {
        result += getSolvedLevelsInGroup(i);
    }
    return result;
}

int getSolvedLevelsInGroup(const int group)
{
    int result = 0;
    for (int i = 1; i <= 16; ++i) {
        if (getLevelStars(group, i) > 0) {
            ++result;
        }
    }
    return result;
}

int getStarsInAllGroups()
{
    int result = 0;
    for (int i = 1; i <= 10; ++i) {
        result += getStarsInGroup(i);
    }
    return result;
}

int getStarsInGroup(const int group)
{
    int result = 0;
    for (int i = 1; i <= 16; ++i) {
        result += getLevelStars(group, i);
    }
    return result;
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

    if (flag) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1);
    } else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
}

bool hasMusicEnabled()
{
    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey("music", true);
}

void setMusicEnabled(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("music", flag);
    settings->flush();

    if (flag) {
        //SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    } else {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
    }
}

} // namespace user