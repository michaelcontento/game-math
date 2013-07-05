#include "GameScene.h"

using namespace cocos2d;

CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    return true;
}