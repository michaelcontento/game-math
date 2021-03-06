#include "user.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <string>
#include <vector>
#include <chrono>
#include <boost/foreach.hpp>
#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include <avalon/utils/platform.h>
#include <avalon/platform/android/gnustl_string_fixes.h>
#include "MyFlurry.h"
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

typedef std::function<void (const int group, const int level)> Callback;
static std::vector<Callback> starCallbacks = {};

} // namespace impl

bool isLevelGroupLocked(const int group)
{
#if AVALON_PLATFORM_IS_TIZEN
    return false;
#else
    if (group == 0) {
        return false;
    }

    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey(impl::getLockedKey(group).c_str(), true);
#endif
}

void setLevelGroupLocked(const int group, const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey(impl::getLockedKey(group).c_str(), flag);
    settings->flush();

    if (!isLevelGroupLocked(1) && !isLevelGroupLocked(2) && !isLevelGroupLocked(3) && !isLevelGroupLocked(4)) {
        setAdsEnabled(false);
    }
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

        auto valueAllLevels = 100.0 / (16.0 * 10) * totalLevels;
        gc.postAchievement("com.coragames.math.ac.alllevels",  valueAllLevels);
    }

    // trigger callbacks
    BOOST_FOREACH (auto& callback, impl::starCallbacks) {
        callback(group, level);
    }

    MyFlurry::logEventWithType(
        std::to_string(group) + "." + std::to_string(level) + ".stars",
        std::to_string(stars)
    );
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
#if AVALON_PLATFORM_IS_TIZEN
    return false;
#else
    auto settings = UserDefault::getInstance();
    auto enabled = settings->getBoolForKey("ads", true);

    if (!enabled) {
        return false;
    }

    using namespace std::chrono;
    static auto installTs = settings->getDoubleForKey("installts", 0);
    auto currentTs = duration_cast<minutes>(system_clock::now().time_since_epoch()).count();

    if (installTs == 0) {
        installTs = currentTs;
        settings->setDoubleForKey("installts", installTs);
        settings->flush();
        return false;
    }

    static auto cooldown = std::stoi(_("config", "adinstalldelayhours").get()) * 60;
    if (installTs + cooldown > currentTs) {
        return false;
    }

    return true;
#endif
}

void setAdsEnabled(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("ads", flag);
    settings->flush();

    avalon::ads::Manager::enabled = flag;
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
        SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    } else {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
    }
}

int getHintKeys()
{
    auto settings = UserDefault::getInstance();
    return settings->getIntegerForKey("hints", 3);
}

void addHintKeys(const int amount)
{
    auto settings = UserDefault::getInstance();
    settings->setIntegerForKey("hints", getHintKeys() + amount);
    settings->flush();
}

bool useBigHintAlert()
{
    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey("bighintalert", true);
}

void setUseBigHintAlert(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("bighintalert", flag);
    settings->flush();
}

bool useVibration()
{
    auto settings = UserDefault::getInstance();
    return settings->getBoolForKey("vibrate", false);
}

void setUseVibration(const bool flag)
{
    auto settings = UserDefault::getInstance();
    settings->setBoolForKey("vibrate", flag);
    settings->flush();
}

void clear()
{
    setUseBigHintAlert(true);

    auto settings = UserDefault::getInstance();
    settings->setIntegerForKey("hints", 3);
    settings->setBoolForKey("ads", true);
    settings->setDoubleForKey("installts", 0);

    for (int i = 0; i <= 10; ++i) {
        for (int j = 0; i <= 16; ++j) {
            settings->setBoolForKey(impl::getLockedKey(i).c_str(), true);
            settings->setIntegerForKey(impl::getStarsKey(i, j).c_str(), 0);
        }
    }

    settings->flush();
}

} // namespace user
