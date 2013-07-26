#include "ToggleButton.h"

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
        ->addTargetedDelegate(this, -90, true);

    if (label) {
        label->setString(getLabel(detectState()).c_str());
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
    if (!helper::isNodeInViewRect(*this)) {
        return false;
    }

    auto size = getContentSize();
    auto bb = Rect(0, 0, size.width, size.height);
    return bb.containsPoint(convertTouchToNodeSpace(touch));
}

void ToggleButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!ccTouchBegan(touch, event)) {
        return;
    }

    auto state = !detectState();
    if (toggleAction(state)) {
        label->setString(getLabel(state).c_str());
    }
}

bool ToggleButton::init()
{
    if (!Node::init()) {
        return false;
    }

    setContentSize({
        config::getFrameSize().width * 0.75,
        75 * config::getScaleFactor()
    });
    
    addBackground();
    addLabel();

    return true;
}

void ToggleButton::addBackground()
{
    auto draw = DrawNode::create();
    addChild(draw);

    auto size = getContentSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};
    
    auto color = color::toRGBA(Color3B(171, 168, 171));
    draw->drawPolygon(verts, 4, color, 0, color);
}

void ToggleButton::addLabel()
{
    label = fonts::createLight(getLabel(detectState()).c_str(), 54);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setPosition(getContentSize() / 2);
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);
    addChild(label);
}
