#include "GameScene.h"

#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "PageManager.h"

using namespace cocos2d;

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

    pageManager = PageManager::create();
    addChild(pageManager);

    pageManager->add("settings", SettingsPage::create());
    pageManager->add("main", MainPage::create());
    pageManager->add("category-01", CategoryPage::create(1));
    pageManager->add("category-02", CategoryPage::create(2));
    pageManager->add("category-03", CategoryPage::create(3));
    pageManager->add("category-04", CategoryPage::create(4));
    pageManager->add("category-05", CategoryPage::create(5));
    pageManager->add("category-06", CategoryPage::create(6));
    pageManager->add("category-07", CategoryPage::create(7));
    pageManager->add("category-08", CategoryPage::create(8));
    pageManager->add("category-09", CategoryPage::create(9));
    pageManager->add("category-10", CategoryPage::create(10));
    pageManager->scrollTo("main", 0);

    return true;
}