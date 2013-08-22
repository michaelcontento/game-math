#include "GameScene.h"

#include <avalon/ads/Manager.h>
#include <avalon/payment.h>
#include <avalon/GameCenter.h>
#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "pages/LockedCategoryPage.h"
#include "pages/MoreGamesPage.h"
#include "PageManager.h"
#include "utils/user.h"

using namespace cocos2d;
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

    auto gc = avalon::GameCenter();
    gc.login();

    pageManager = PageManager::create();
    addChild(pageManager);

    pageManager->add("settings", SettingsPage::create());
    pageManager->add("main", MainPage::create());
    addCategoryPages(*pageManager);
    pageManager->add("moregames", MoreGamesPage::create());
    pageManager->scrollTo("main", 0);

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