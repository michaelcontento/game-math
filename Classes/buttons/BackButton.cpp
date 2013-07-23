#include "BackButton.h"

#include "../PageManager.h"
#include "../utils/config.h"

using namespace cocos2d;

void BackButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);
}

void BackButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool BackButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return boundingBox().containsPoint(touch->getLocation());
}

void BackButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    PageManager::shared().scrollUp();
}

bool BackButton::init()
{
    if (!Node::init()) {
        return false;
    }

    auto btn = Sprite::create("arrow.png");
    btn->setAnchorPoint({0, 0});
    btn->setScale(config::getScaleFactor());

    addChild(btn);
    setContentSize(btn->getContentSize() * config::getScaleFactor());

    return true;
}