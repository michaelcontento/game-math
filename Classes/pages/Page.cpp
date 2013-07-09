#include "Page.h"

#include "../PageManager.h"

using namespace cocos2d;

void Page::setBackground(const cocos2d::ccColor3B& color)
{
    auto oldBg = getChildByTag(TAG_BACKGROUND);

    if (oldBg) {
        auto bg = dynamic_cast<LayerColor*>(oldBg);
        bg->setColor(color);
    } else {
        auto bg = LayerColor::create({color.r, color.g, color.b, 255});
        bg->setTag(TAG_BACKGROUND);
        addChild(bg);
    }
}

bool Page::init()
{
    if (!Layer::init()) {
        return false;
    }

    setTouchEnabled(true);

    return true;
}

float Page::getVisibleWidth() const
{
    auto posX = getPositionX();
    if (posX <= 0) {
        return std::min(getContentSize().width, posX + getContentSize().width);
    } else {
        return std::max(0.f, getContentSize().width - posX);
    }
}

bool Page::isVisible() const
{
    auto posX = getPositionX();

    if (posX <= 0 - getContentSize().width) {
        return false;
    }

    if (posX >= getContentSize().width) {
        return false;
    }

    return true;
}


#pragma -
#pragma mark Touch handling

void Page::registerWithTouchDispatcher()
{
    Director::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, -1, false);
}

bool Page::ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    return true;
}

void Page::ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    if (manager && manager->hasTouchHandled(pTouch, pEvent)) {
        //
    } else {
        onTouch(pTouch, pEvent);
    }
}