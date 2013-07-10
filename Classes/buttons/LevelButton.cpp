#include "LevelButton.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../pages/Page.h"
#include "../pages/GamePage.h"
#include "../PageManager.h"

using namespace cocos2d;

LevelButton* LevelButton::create(const short number, const Page* const parentPage)
{
    LevelButton* pRet = new LevelButton();
    if (pRet && pRet->init(number, parentPage)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LevelButton::init(const short number, const Page* const parentPage)
{
    if (!Node::init()) {
        return false;
    }

    this->number = number;
    this->parentPage = parentPage;

    configureSize();
    addBackground();
    addNumber();

    return true;
}

void LevelButton::configureSize()
{
    auto size = 150 * config::getScaleFactor();
    setContentSize({size, size});
}

void LevelButton::addBackground()
{
    auto draw = DrawNode::create();
    addChild(draw);

    auto size = getContentSize().width;
    Point verts[] = {{0, 0}, {0, size}, {size, size}, {size, 0}};

    auto color = getBackgroundColorFromParentPage();
    draw->drawPolygon(verts, 4, color, 1, color);
}

const cocos2d::ccColor4F LevelButton::getBackgroundColorFromParentPage() const
{
    auto hsvColor = color::toHSV(parentPage->getBackground());

    auto step = config::getHsvColorStep();
    hsvColor.h += step.h;
    hsvColor.s += step.s;
    hsvColor.v += step.v;

    return color::toRGBA(hsvColor);
}

void LevelButton::addNumber()
{
    auto label = fonts::createNormal(std::to_string(number).c_str(), 67);
    addChild(label);

    // color
    label->setColor(ccWHITE);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    label->setPosition(getContentSize() / 2);
}

void LevelButton::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!hasBeenTouched(touch, event)) {
        return;
    }

    PageManager::shared().scrollDown(GamePage::create());
}

bool LevelButton::hasBeenTouched(cocos2d::Touch* touch, cocos2d::Event* event)
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