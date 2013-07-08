#include "GameScene.h"

#include "pages/MainPage.h"

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

    pageManager->add("main", std::unique_ptr<Page>(MainPage::create()));

    return true;
}