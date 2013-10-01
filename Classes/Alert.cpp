#include "Alert.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "utils/config.h"
#include "utils/color.h"
#include "utils/fonts.h"

using namespace cocos2d;

bool Alert::init()
{
    if (!Layer::init()) {
        return false;
    }

    // background
    draw = DrawNode::create();
    addChild(draw);

    const float height = 200 * config::getScaleFactor() * 0.5;
    const float width = config::getFrameSize().width;
    Point verts[] = {{0, -height}, {0, height}, {width, height}, {width, -height}};

    auto color = color::toRGBA(Color3B(87, 87, 87));
    draw->drawPolygon(verts, 4, color, 1, color);

    // configure container
    setContentSize({width, height});
    setAnchorPoint({0, 0.5});
    setPositionY(config::getFrameSize().height / 2);

    // tap to continue
    tap = fonts::createLight(_("general", "taptocontinue").get(), 42, TextHAlignment::CENTER, TextVAlignment::TOP, 66);
    addChild(tap);

    tap->setAnchorPoint({0.5, 1});
    tap->setPositionX(getContentSize().width + tap->getContentSize().width);
    tap->setPositionY(getContentSize().height / 2 * -1);

    return true;
}

void Alert::show(const std::function<void ()> callback, const bool instant)
{
    this->callback = callback;
    visible = true;
    touchable = false;

    float instaMod = instant ? 0 : 1;

    draw->setScaleY(0);
    draw->stopAllActions();
    draw->runAction(EaseInOut::create(ScaleTo::create(config::getAlertFadeTime() * instaMod, 1, 1), 3));

    Node* bottomAnimationNode = nullptr;
    if (tap) {
        bottomAnimationNode = tap;
    }

    if (buttonContainer) {
        bottomAnimationNode = buttonContainer;
        
        float nextPosX = 0;
        const float padding = 35 * config::getScaleFactor();
        for (auto& pairs : buttons) {
            auto btn = pairs.first;
            btn->setPositionX(nextPosX);
            nextPosX += btn->getContentSize().width + padding;
        }

        nextPosX -= padding;
        const auto size = Size(nextPosX, buttons.front().first->getContentSize().height);
        buttonContainer->setContentSize(size);
        buttonContainer->setAnchorPoint({0.5, 0});
        buttonContainer->setPositionX(getContentSize().width + (size.width / 2) + 10);
    }
    
    if (bottomAnimationNode) {
        bottomAnimationNode->stopAllActions();
        bottomAnimationNode->runAction(Sequence::create(
            CallFunc::create([this]() {
                setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
                setSwallowsTouches(true);
                setTouchEnabledWithFixedPriority(-200);
                setKeyboardEnabled(true);
            }),
            DelayTime::create(config::getAlertFadeTime() * 0.2 * instaMod),
            EaseInOut::create(
                Spawn::create(
                    MoveTo::create(config::getAlertFadeTime() * instaMod, {getContentSize().width / 2, bottomAnimationNode->getPositionY()}),
                    FadeIn::create(config::getAlertFadeTime() * instaMod),
                    NULL
                ),
                3
            ),
            CallFunc::create([this]() {
                touchable = true;
            }),
            NULL
        ));
    }

    if (desc) {
        desc->stopAllActions();
        desc->runAction(Sequence::create(
            DelayTime::create(config::getAlertFadeTime() * 0.2 * instaMod),
            EaseInOut::create(MoveTo::create(config::getAlertFadeTime() * instaMod, {getContentSize().width / 2, desc->getPositionY()}), 3),
            NULL
        ));
    }
}

void Alert::hide(const bool instant)
{
    unschedule(schedule_selector(Alert::onTick));
    
    visible = true;
    touchable = false;

    float instaMod = instant ? 0 : 1;

    draw->stopAllActions();
    draw->runAction(Sequence::create(
        DelayTime::create(config::getAlertFadeTime() * 0.2 * instaMod),
        EaseInOut::create(ScaleTo::create(config::getAlertFadeTime() * instaMod, 1, 0), 3),
        CallFunc::create([this]() {
            visible = false;
            setTouchEnabled(false);
            setKeyboardEnabled(false);
            if (this->buttonCb) this->buttonCb();
            this->callback();
        }),
        RemoveSelf::create(),
        NULL
    ));

    Node* bottomAnimationNode = nullptr;
    if (tap) {
        bottomAnimationNode = tap;
    }    
    if (buttonContainer) {
        bottomAnimationNode = buttonContainer;
    }

    if (bottomAnimationNode) {
        auto pos = Point(bottomAnimationNode->getContentSize().width * -1, bottomAnimationNode->getPositionY());
        bottomAnimationNode->stopAllActions();
        bottomAnimationNode->runAction(EaseInOut::create(
            Spawn::create(
                MoveTo::create(config::getAlertFadeTime() * instaMod, pos),
                FadeOut::create(config::getAlertFadeTime() * instaMod),
                NULL
            ),
            3
        ));
    }

    if (desc) {
        auto pos = Point(getContentSize().width + desc->getContentSize().width, desc->getPositionY());
        desc->stopAllActions();
        desc->runAction(EaseInOut::create(MoveTo::create(config::getAlertFadeTime() * instaMod, pos), 3));
    }
}

void Alert::enableCloseOnTap(const bool flag)
{
    closeOnTap = flag;
    if (tap)  tap->setVisible(flag);
    if (desc) desc->setAnchorPoint({0.5, flag ? 0.3f : 0.5f});
}

void Alert::setDescription(const std::string& description)
{
    if (!desc) {
        desc = fonts::createLight(description.c_str(), 72, TextHAlignment::CENTER, TextVAlignment::CENTER);
        addChild(desc);

        desc->setAnchorPoint({0.5, 0.3});
        desc->setPosition(Point(getContentSize()));
        desc->setPositionY(desc->getPositionY() - getContentSize().height);
    }

    desc->setString(description.c_str());
    desc->setPositionX(desc->getContentSize().width * -0.5);

    const auto textSize = desc->getContentSize();
    const static auto maxWidth = config::getFrameSize().width * 0.9;
    const static auto maxHeight = getContentSize().height * 1.2;

    if (textSize.width > maxWidth) {
        desc->setScale(maxWidth / textSize.width);
    }
    if (textSize.height > maxHeight) {
        desc->setScale(MIN(desc->getScale(), maxHeight / textSize.height));
    }
}

bool Alert::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void Alert::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!touchable || !closeOnTap) {
        return;
    }

    if (buttons.empty()) {
        hide();
        SimpleAudioEngine::getInstance()->playEffect("click.mp3");
        return;
    }

    for (auto& pair : buttons) {
        auto btn = pair.first;
        auto cb = pair.second;

        auto location = convertTouchToNodeSpace(touch);
        location.x = buttonContainer->convertToNodeSpace(location).x;

        if (btn->getBoundingBox().containsPoint(location)) {
            buttonCb = cb;
            hide();
            SimpleAudioEngine::getInstance()->playEffect("click.mp3");
            return;
        }
    }
}

void Alert::visit()
{
    if (visible) {
        Layer::visit();
    }
}

void Alert::setTimeout(const float secs, std::function<void ()> callback)
{
    timeoutSecs = secs;
    timeoutCb = callback;

    schedule(schedule_selector(Alert::onTick), 0.1);
}

void Alert::onTick(const float dt)
{
    if (!visible) {
        return;
    }
    
    timeoutSecs -= dt;
    
    if (timeoutSecs <= 0) {
        timeoutCb();
        unschedule(schedule_selector(Alert::onTick));
    }
}

void Alert::addButton(const std::string& description, std::function<void ()> callback)
{
    if (tap) {
        tap->removeFromParent();
        tap = nullptr;
    }

    if (!buttonContainer) {
        buttonContainer = Node::create();
        addChild(buttonContainer);
    }

    auto node = Node::create();
    buttonContainer->addChild(node);
    buttons.push_back(std::make_pair(node, callback));

    const auto size = Size(220, 40) * config::getScaleFactor();
    node->setContentSize(size);

    node->setAnchorPoint({0, 1});
    node->setPositionY(getContentSize().height / 2 * -1);

    auto draw = DrawNode::create();
    node->addChild(draw);
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};
    draw->drawPolygon(verts, 4, color::toRGBA(Color3B({55, 55, 55})), 0, {});

    auto label = fonts::createLight(description, 36, TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setPosition(Point(size / 2));
    node->addChild(label);
}

void Alert::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (keyCode != EventKeyboard::KeyCode::KEY_BACKSPACE) {
        Layer::onKeyReleased(keyCode, event);
        return;
    }

    if (!visible) {
        Layer::onKeyReleased(keyCode, event);
        return;
    }

    if (!buttons.empty()) {
        buttonCb = buttons.front().second;
    }
    hide();
    event->stopPropagation();
}