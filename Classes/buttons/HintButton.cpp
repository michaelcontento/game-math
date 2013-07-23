#include "HintButton.h"

#include "../utils/fonts.h"
#include "../pages/GamePage.h"

using namespace cocos2d;

HintButton* HintButton::create(GamePage& game)
{
    HintButton* pRet = new HintButton();
    if (pRet && pRet->init(game)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool HintButton::init(GamePage& game)
{
    if (!Node::init()) {
        return false;
    }

    this->game = &game;

    addLabel();

    return true;
}

void HintButton::addLabel()
{
    label = fonts::createLight("!5", 48);
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentTop);

    // update content size of the container
    setContentSize(label->getContentSize());
}

void HintButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);
}

void HintButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool HintButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return boundingBox().containsPoint(touch->getLocation());
}

void HintButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!game->isStarted() || game->isTimeover()) {
        return;
    }

    game->revealHint();
}