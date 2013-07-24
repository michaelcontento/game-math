#include "GameScene.h"

#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "pages/LockedCategoryPage.h"
#include "PageManager.h"
#include "utils/user.h"

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
    addCategoryPages(*pageManager);
    pageManager->scrollTo("category-1", 0);

    return true;
}

void GameScene::addCategoryPages(PageManager& pageManager) const
{
    std::string name = "category-";
    for (int i = 1; i <= 10; i += 2) {
        if (user::hasPurchased(i)) {
            pageManager.add(name + std::to_string(i + 0), CategoryPage::create(i + 0));
            pageManager.add(name + std::to_string(i + 1), CategoryPage::create(i + 1));
        } else {
            pageManager.add(name + std::to_string(i), LockedCategoryPage::create(i));
        }
    }
}