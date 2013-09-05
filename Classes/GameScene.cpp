#include "GameScene.h"

#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include <avalon/i18n/Localization.h>
#include "SimpleAudioEngine.h"
#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "pages/LockedCategoryPage.h"
#include "pages/MoreGamesPage.h"
#include "PageManager.h"
#include "utils/user.h"
#include "utils/config.h"

//#define PROFILE 100

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

    initLocalization();
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
    pageManager->scrollTo("main", 0);

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

    if (!FileUtils::getInstance()->isFileExist(lang.c_str())) {
        return;
    }

    loca->addLanguage(lang.c_str());
    loca->setCurrentLanguage(lang.c_str());
}