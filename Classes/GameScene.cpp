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
    pageManager->add("category-01", CategoryPage::create());
    pageManager->scrollTo("main", 0);

    return true;
}