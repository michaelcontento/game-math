#include "GameScene.h"

#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include "SimpleAudioEngine.h"
#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "pages/LockedCategoryPage.h"
#include "pages/MoreGamesPage.h"
#include "PageManager.h"
#include "utils/user.h"
#include "utils/config.h"

#define PROFILE

using namespace cocos2d;
using namespace CocosDenshion;
using namespace avalon;

Scene* GameScene::scene()
{
    Scene* scene = Scene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }

    initAds();
    initPayment();
    initGameCenter();
    initSoundAndMusic();

    pageManager = PageManager::create();
    addChild(pageManager);
    pageManager->add("settings", SettingsPage::create());
    pageManager->add("main", MainPage::create());
    addCategoryPages(*pageManager);
    pageManager->add("moregames", MoreGamesPage::create());
    pageManager->scrollTo("category-8", 0);

#ifdef PROFILE
    for (int group = 7; group <= 10; ++group) {
        if (group == 9 || group == 10) continue;
        
        for (int level = 1; level <= 16; ++level) {
            auto generator = config::getGenerator(group, level);
            log(">>> PROFILING: %d/%d", group, level);
            ProfilingBeginTimingBlock("> TOTAL");
            for (int i = 0; i <= 30000; ++i) {
                ProfilingBeginTimingBlock("> QUESTIONS");
                generator();
                ProfilingEndTimingBlock("> QUESTIONS");
            }
            ProfilingEndTimingBlock("> TOTAL");
            Profiler::getInstance()->displayTimers();
        }
    }
#endif

    return true;
}

void GameScene::addCategoryPages(PageManager& pageManager) const
{
    std::string name = "category-";
    for (int i = 1; i <= 10; i += 2) {
        const auto group = std::ceil(i / 2.f) - 1;
        if (!user::isLevelGroupLocked(group)) {
            pageManager.add(name + std::to_string(i + 0), CategoryPage::create(i + 0));
            pageManager.add(name + std::to_string(i + 1), CategoryPage::create(i + 1));
        } else {
            pageManager.add(name + std::to_string(i), LockedCategoryPage::create(i));
        }
    }
}

void GameScene::initAds() const
{
    avalon::ads::Manager::enabled = user::hasAdsEnabled();
    if (avalon::ads::Manager::enabled) {
        avalon::ads::Manager::initWithIniFile("ads.ini");
        avalon::ads::Manager::startService();
    }
}

void GameScene::initPayment()
{
    payment::Loader loader("payment.ini");
    payment::Loader::globalManager = loader.getManager();
    payment::Loader::globalManager->startService();
}

void GameScene::initGameCenter()
{
    auto gc = avalon::GameCenter();
    gc.login();
}

void GameScene::initSoundAndMusic()
{
    if (user::hasMusicEnabled()) {
        //SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    }
    
    if (!user::hasSoundEnabled()) {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
}

// ====== EQUATIONS
// AUCH NEGATIVE ZAHLEN!

// Q: x + 3 = 5; x = ?
// A: 2

// Q: x - 3 = 5; x = ?
// A: 2

// Q: 3x *ADD/SUB* 8 = 19; x = ?
// A: 9

// Q: 4x *ADD/SUB* 6 = 6x *ADD/SUB* 16; x = ?
// A: 5

// Q: 7(x *ADD/SUB* 5) = 28; x = ?
// A: 9

// Q: -24 / (x *ADD/SUB* 8) = 4; x = ?
// A: 2

// Q: x *ADD/SUB* y = -2; y = 1; x = ?
// A: