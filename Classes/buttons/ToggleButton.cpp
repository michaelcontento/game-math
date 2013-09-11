#include "ToggleButton.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "../PageManager.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../utils/config.h"
#include "../utils/helper.h"

using namespace cocos2d;

void ToggleButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, -90, false);

    if (label) {
        label->setString(_("settings", getLabel(detectState()).c_str()).get().c_str());
    }
}

void ToggleButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool ToggleButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    const auto size = getContentSize();
    const auto bb = Rect(0, 0, size.width, size.height);
    return bb.containsPoint(convertTouchToNodeSpace(touch));
}

void ToggleButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!ccTouchBegan(touch, event)) {
        return;
    }

    const auto startDelta = touch->getLocation() - touch->getStartLocation();
    if (PageManager::shared().hasControl()) {
        // user is scrolling through the pages
        return;
    } else if (startDelta.getLength() >= config::getTouchIgnoreLength()) {
        // not a valid scrolling touch (maybe a old animation was running?) but
        // it moved to far to be a valid touch
        return;
    }

    const auto state = !detectState();
    if (toggleAction(state)) {
        label->setString(_("settings", getLabel(state).c_str()).get().c_str());
    }
}

bool ToggleButton::init()
{
    if (!Node::init()) {
        return false;
    }

    setContentSize(config::getAnswerButtonSize());
    
    addBackground();
    addLabel();

    return true;
}

void ToggleButton::addBackground()
{
    const auto draw = DrawNode::create();
    addChild(draw);

    const auto size = getContentSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};
    
    const auto color = color::toRGBA(Color3B(171, 168, 171));
    draw->drawPolygon(verts, 4, color, 0, {});
}

void ToggleButton::addLabel()
{
    label = fonts::createLight(getLabel(detectState()).c_str(), 52 * config::getScaleFactorHeightMagic(), TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setPosition(Point(getContentSize() / 2));
    addChild(label);
}
