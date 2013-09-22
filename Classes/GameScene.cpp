#include "GameScene.h"

#include <thread>
#include <chrono>
#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include <avalon/i18n/Localization.h>
#include <avalon/utils/platform.h>
#include <AssetsManager/AssetsManager.h>
#include "SimpleAudioEngine.h"
#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "pages/AboutPage.h"
#include "pages/LockedCategoryPage.h"
#include "pages/MoreGamesPage.h"
#include "PageManager.h"
#include "utils/user.h"
#include "utils/config.h"
#include "utils/android_fixes.h"

//#define PROFILE 100
//#define DEVASSETS

using namespace cocos2d;
using namespace CocosDenshion;
using namespace avalon;

std::string GameScene::searchPath {};

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

    initAssetsSearchpath();
    initAssets();
    initLocalization();
    initPages();
    profile();

#if AVALON_PLATFORM_IS_IOS
    std::thread t(threadInit);
    t.detach();
#else
    threadInit();
#endif

    return true;
}

void GameScene::threadInit()
{
#if AVALON_PLATFORM_IS_IOS
    // don't interfere with the initial animation
    std::this_thread::sleep_for(std::chrono::seconds(1));
#endif

    initPayment();
    initGameCenter();
    initAds();
    initSoundAndMusic();

#if !AVALON_PLATFORM_IS_ANDROID
    // this is not that important and can wait a few more seconds
    // for the whole system to stabilize / get ready
    std::this_thread::sleep_for(std::chrono::seconds(2));
    updateAssets();
#endif
}

void GameScene::initPages()
{
    pageManager = PageManager::create();
    addChild(pageManager);
    pageManager->add("about", AboutPage::create());
    pageManager->add("settings", SettingsPage::create());
    pageManager->add("main", MainPage::create());
    addCategoryPages(*pageManager);
    pageManager->add("moregames", MoreGamesPage::create());
    pageManager->scrollTo("main", 0);
}

void GameScene::profile()
{
#ifdef PROFILE
    for (int group = 1; group <= 10; ++group) {
        for (int level = 1; level <= 16; ++level) {
            auto generator = config::getGenerator(group, level);
            log(">>> PROFILING: %d/%d", group, level);
            ProfilingBeginTimingBlock("> TOTAL");
            for (int i = 0; i <= PROFILE; ++i) {
                ProfilingBeginTimingBlock("> QUESTIONS");
                generator();
                ProfilingEndTimingBlock("> QUESTIONS");
            }
            ProfilingEndTimingBlock("> TOTAL");
            Profiler::getInstance()->displayTimers();
        }
    }
#endif
}

void GameScene::initAssets()
{
    SpriteFrameCache::getInstance()
        ->addSpriteFramesWithFile("assets.plist");
}

void GameScene::initAssetsSearchpath()
{
    searchPath = FileUtils::getInstance()->getWritablePath();
    
    auto searchPaths = FileUtils::getInstance()->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), searchPath);
    FileUtils::getInstance()->setSearchPaths(searchPaths);
}

void GameScene::updateAssets()
{
    auto mgr = new extension::AssetsManager(
#ifdef DEVASSETS
        "http://appdata.coragames.com.s3-website-eu-west-1.amazonaws.com/math/package.zip",
        "http://appdata.coragames.com.s3-website-eu-west-1.amazonaws.com/math/version",
#else
        "http://appdata.coragames.com/math/package.zip",
        "http://appdata.coragames.com/math/version",
#endif
        searchPath.c_str()
    );
    mgr->setConnectionTimeout(5);
    mgr->update();
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

void GameScene::initAds()
{
    avalon::ads::Manager::initWithIniFile("ads.ini");
    avalon::ads::Manager::startService();
    avalon::ads::Manager::enabled = user::hasAdsEnabled();
}

void GameScene::initPayment()
{
    payment::Loader loader("payment.ini");
    payment::Loader::globalManager = loader.getManager();

    auto settings = dynamic_cast<SettingsPage*>(PageManager::shared().getPage("settings"));
    payment::Loader::globalManager->delegate = settings;

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
        SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2);
    }

    if (user::hasSoundEnabled()) {
        SimpleAudioEngine::getInstance()->preloadEffect("click.mp3");
        SimpleAudioEngine::getInstance()->preloadEffect("solved.mp3");
        SimpleAudioEngine::getInstance()->preloadEffect("lost.mp3");
        SimpleAudioEngine::getInstance()->preloadEffect("wrong.mp3");
    } else {
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
}

void GameScene::initLocalization()
{
    auto loca = &avalon::i18n::Localization::getInstance();
    loca->addLanguage("en.ini");
    loca->setCurrentLanguage("en.ini");
    loca->setDefaultLanguage("en.ini");

    auto langId = Application::getInstance()->getCurrentLanguage();
    std::string lang;
    if (langId == LanguageType::ARABIC) {
        lang = "ar";
    } else if (langId == LanguageType::CHINESE) {
        lang = "cn";
    } else if (langId == LanguageType::ENGLISH) {
        // already set above
        return;
    } else if (langId == LanguageType::FRENCH) {
        lang = "fr";
    } else if (langId == LanguageType::GERMAN) {
        lang = "de";
    } else if (langId == LanguageType::HUNGARIAN) {
        lang = "hr";
    } else if (langId == LanguageType::ITALIAN) {
        lang = "it";
    } else if (langId == LanguageType::JAPANESE) {
        lang = "ja";
    } else if (langId == LanguageType::KOREAN) {
        lang = "ko";
    } else if (langId == LanguageType::PORTUGUESE) {
        lang = "pt";
    } else if (langId == LanguageType::RUSSIAN) {
        lang = "ru";
    } else if (langId == LanguageType::SPANISH) {
        lang = "es";
    } else if (langId == LanguageType::NORWEGIAN) {
        lang = "nb";
    } else if (langId == LanguageType::POLISH) {
        lang = "pl";
    } else {
        // unknown id .. stick to the default (en)
        return;
    }
    lang += ".ini";

    // search for the language file in ALL possible paths
    bool found = false;
    for (const auto path : FileUtils::getInstance()->getSearchPaths()) {
        if (FileUtils::getInstance()->isFileExist(path + lang)) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }

    loca->addLanguage(lang.c_str());
    loca->setCurrentLanguage(lang.c_str());
}