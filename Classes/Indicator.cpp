#include "Indicator.h"

#include "PageManager.h"

using namespace cocos2d;

void Indicator::addDot()
{
    auto sprite = Sprite::createWithSpriteFrameName("indicator.png");
    sprite->setAnchorPoint(Point::ZERO);
    sprite->setOpacity(0);
    sprite->setPositionX(nextPosX);
    sprite->setPositionY(posY);
    sprite->setScale(std::min(1.f, config::getScaleFactor()) * 0.8);
    addChild(sprite);

    nextPosX += (sprite->getContentSize().width * sprite->getScaleX()) + padding;

    auto size = getContentSize();
    size.width = nextPosX - padding;
    setContentSize(size);

    updateOpacity();
}

void Indicator::updateOpacity()
{
    const auto activeIndex = PageManager::shared().getCurrentPageIndex();
    int currentIndex = 0;

    for (auto node : *getChildren()) {
        auto sprite = dynamic_cast<Sprite*>(node);
        if (!sprite) {
            continue;
        }

        if (currentIndex == activeIndex) {
            sprite->setOpacity(255 * opacityActive);
        } else {
            sprite->setOpacity(255 * opacityInactive);
        }
        ++currentIndex;
    }
}