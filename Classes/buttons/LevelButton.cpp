#include "LevelButton.h"

#include <functional>
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../utils/user.h"
#include "../pages/Page.h"
#include "../pages/GamePage.h"
#include "../PageManager.h"

using namespace cocos2d;

LevelButton* LevelButton::create(const int group, const int level, const Page& parentPage)
{
    LevelButton* pRet = new LevelButton();
    if (pRet && pRet->init(group, level, parentPage)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LevelButton::init(const int group, const int level, const Page& parentPage)
{
    if (!Node::init()) {
        return false;
    }

    this->group = group;
    this->level = level;
    this->parentPage = &parentPage;

    configureSize();
    addBackground();
    addNumber();
    changeOpacity(group, level);

    user::addStarChangeCallback(std::bind(&LevelButton::changeOpacity, this, std::placeholders::_1, std::placeholders::_2));
    
    return true;
}

void LevelButton::changeOpacity(const int group, const int level)
{
    if (ignoreNotification(group, level)) {
        return;
    }

    const auto starCount = user::getLevelStars(this->group, this->level);
    if (level == 1 || starCount > 0) {
        unlocked = true;
    } else {
        unlocked = (user::getLevelStars(this->group, this->level - 1) > 0);
    }

    refillStarContainerIfRequired(starCount);
    updateOpacity();
}

bool LevelButton::ignoreNotification(const int group, const int level)
{
    if (this->group != group) {
        return true;
    }

    if (this->level != level && this->level != level + 1) {
        return true;
    }

    return false;
}

void LevelButton::refillStarContainerIfRequired(const int amount)
{
    if (amount <= 0) {
        return;
    }

    initializeStarContainerIfRequired();

    if (amount == stars.size()) {
        return;
    }

    stars.clear();
    starContainer->removeAllChildren();
    
    fonts::fillStarContainer(
        *starContainer,
        stars,
        amount,
        parentPage->getBackground()
    );
}

void LevelButton::updateOpacity()
{
    addBackground();

    if (!unlocked) {
        label->setOpacity(127);
    } else {
        label->setOpacity(255);
    }
}

void LevelButton::initializeStarContainerIfRequired()
{
    if (starContainer) {
        return;
    }
    
    starContainer = Node::create();
    addChild(starContainer);

    starContainer->setAnchorPoint({0.5, 1});
    starContainer->setPosition(label->getPosition());

    const auto starOffset = 30 * config::getScaleFactor();
    starContainer->setPositionY(starContainer->getPositionY() - starOffset);
    
    const auto labelOffset = 10 * config::getScaleFactor();
    label->setPositionY(label->getPositionY() + labelOffset);
}

void LevelButton::configureSize()
{
    const auto size = 150 * config::getScaleFactor();
    setContentSize({size, size});
}

void LevelButton::addBackground()
{
    if (!draw) {
        draw = DrawNode::create();
        addChild(draw);
    } else {
        draw->clear();
    }

    const auto size = getContentSize().width;
    Point verts[] = {{0, 0}, {0, size}, {size, size}, {size, 0}};

    auto color = getBackgroundColorFromParentPage();
    if (!unlocked) {
        color.a = 0.5;
    }
    draw->drawPolygon(verts, 4, color, 0, {});
}

cocos2d::Color4F LevelButton::getBackgroundColorFromParentPage() const
{
    auto hsvColor = color::toHSV(parentPage->getBackground());

    const auto step = config::getHsvColorStep();
    hsvColor.h = fmod(hsvColor.h + step.h, 360.0);
    hsvColor.s = fmin(1.0, fmax(0.0, hsvColor.s + step.s));
    hsvColor.v = fmin(1.0, fmax(0.0, hsvColor.v + step.v));

    return color::toRGBA(hsvColor);
}

void LevelButton::addNumber()
{
    label = fonts::createLight(std::to_string(level).c_str(), 67);
    addChild(label);

    // color
    label->setColor(Color3B::WHITE);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);

    // positioning
    label->setPosition(Point(getContentSize() / 2));
}

void LevelButton::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    if (unlocked && hasBeenTouched(touch, event)) {
        PageManager::shared().scrollDown(
            GamePage::create(group, level, *parentPage)
        );
    }
}

bool LevelButton::hasBeenTouched(cocos2d::Touch& touch, cocos2d::Event& event)
{
    auto localTouch = convertTouchToNodeSpace(&touch);

    if (localTouch.x < 0 || localTouch.x > getContentSize().width) {
        return false;
    }
    
    if (localTouch.y < 0 || localTouch.y > getContentSize().height) {
        return false;
    }

    return true;
}