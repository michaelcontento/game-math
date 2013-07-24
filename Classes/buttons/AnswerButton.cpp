#include "AnswerButton.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../pages/Page.h"
#include "../pages/GamePage.h"
#include "../PageManager.h"

using namespace cocos2d;

void AnswerButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);
}

void AnswerButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool AnswerButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return isAnswerVisible() && boundingBox().containsPoint(touch->getLocation());
}

bool AnswerButton::isAnswerVisible() const
{
    return label->getOpacity() == 255;
}

void AnswerButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isAnswerVisible()) {
        return;
    }
    
    if (isRight) {
        page->answeredRight();
    } else {
        page->answeredWrong();
    }
}

AnswerButton* AnswerButton::create(GamePage& page, const cocos2d::Color3B& color)
{
    AnswerButton* pRet = new AnswerButton();
    if (pRet && pRet->init(page, color)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool AnswerButton::init(GamePage& page, const cocos2d::Color3B& color)
{
    if (!Node::init()) {
        return false;
    }

    this->page = &page;

    setContentSize(config::getAnswerButtonSize());
    addBackground(color);
    addLabel();

    return true;
}

void AnswerButton::setAnswerString(const std::string& text)
{
    label->setString(text.c_str());
}

void AnswerButton::setIsRight(const bool flag)
{
    isRight = flag;
}

void AnswerButton::fadeOutAnswer(const float duration)
{
    label->stopAllActions();
    label->runAction(FadeOut::create(duration));
}

void AnswerButton::hideAnswer()
{
    label->stopAllActions();
    label->setOpacity(0);
}

void AnswerButton::showAnswer()
{
    label->stopAllActions();
    label->setOpacity(255);
}

void AnswerButton::addBackground(const cocos2d::Color3B& color)
{
    auto draw = DrawNode::create();
    addChild(draw);

    auto size = getContentSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};

    draw->drawPolygon(verts, 4, color::toRGBA(color), 0, {});
}

void AnswerButton::addLabel()
{
    label = fonts::createLight("", 72);
    addChild(label);

    // color
    label->setColor(Color3B::WHITE);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    label->setPosition(getContentSize() / 2);
}