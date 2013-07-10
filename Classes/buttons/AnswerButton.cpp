#include "AnswerButton.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../pages/Page.h"
#include "../pages/GamePage.h"
#include "../PageManager.h"

using namespace cocos2d;

AnswerButton* AnswerButton::create(const cocos2d::Color3B& color)
{
    AnswerButton* pRet = new AnswerButton();
    if (pRet && pRet->init(color)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool AnswerButton::init(const cocos2d::Color3B& color)
{
    if (!Node::init()) {
        return false;
    }

    configureSize();
    addBackground(color);
    addLabel();

    return true;
}

void AnswerButton::configureSize()
{
    auto size = 80 * config::getScaleFactor();
    setContentSize({config::getFrameSize().width, size});
}

void AnswerButton::addBackground(const cocos2d::Color3B& color)
{
    auto draw = DrawNode::create();
    addChild(draw);

    auto size = getContentSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};

    Color4F color4f = {color.r / 255.0, color.g / 255.0, color.b / 255.0, 1};
    draw->drawPolygon(verts, 4, color4f, 1, color4f);
}

void AnswerButton::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!hasBeenTouched(touch, event)) {
        return;
    }
}

bool AnswerButton::hasBeenTouched(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto localTouch = convertTouchToNodeSpace(touch);

    if (localTouch.x < 0 || localTouch.x > getContentSize().width) {
        return false;
    }

    if (localTouch.y < 0 || localTouch.y > getContentSize().height) {
        return false;
    }
    
    return true;
}

void AnswerButton::addLabel()
{
    auto label = fonts::createLight("⅝", 72);
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