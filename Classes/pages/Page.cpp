#include "Page.h"

#include "../PageManager.h"
#include "../utils/config.h"

using namespace cocos2d;

void Page::setBackground(const cocos2d::Color3B& color)
{
    this->color = color;

    if (!background) {
        background = LayerColor::create({color.r, color.g, color.b, 255});
        addChild(background);
    } else {
        background->setColor(color);
    }
}

const cocos2d::Color3B& Page::getBackground() const
{
    return color;
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
    const auto posX = getPositionX();
    if (posX <= 0) {
        return std::min(getContentSize().width, posX + getContentSize().width);
    } else {
        return std::max(0.f, getContentSize().width - posX);
    }
}

bool Page::isVisible() const
{
    return PageManager::shared().isPageVisible(*this);
}

void Page::visit()
{
    if (isVisible()) {
        Layer::visit();
    }
}

#pragma -
#pragma mark Touch handling

void Page::registerWithTouchDispatcher()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, false);
}

bool Page::ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    return isVisible();
}

void Page::ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    auto startDelta = pTouch->getLocation() - pTouch->getStartLocation();

    if (PageManager::shared().hasControl()) {
        // user is scrolling through the pages
    } else if (!isVisible()) {
        // we're now longer visible!
    } else if (startDelta.getLength() >= config::getTouchIgnoreLength()) {
        // not a valid scrolling touch (maybe a old animation was running?) but
        // it moved to far to be a valid touch
    } else {
        onTouch(*pTouch, *pEvent);
    }
}