#include "Alert.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

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

    auto height = 200 * config::getScaleFactor() * 0.5;
    auto width = config::getFrameSize().width;
    Point verts[] = {{0, -height}, {0, height}, {width, height}, {width, -height}};

    auto color = color::toRGBA(Color3B(87, 87, 87));
    draw->drawPolygon(verts, 4, color, 1, color);

    // configure container
    setContentSize({width, height});
    setAnchorPoint({0, 0.5});
    setPositionY(config::getFrameSize().height / 2);

    // tap to continue
    tap = fonts::createLight(_("general", "taptocontinue").get(), 36);
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
        const float padding = 25 * config::getScaleFactor();
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
                Director::getInstance()
                    ->getTouchDispatcher()
                    ->addTargetedDelegate(this, -100, true);
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

void Alert::hide()
{
    unschedule(schedule_selector(Alert::onTick));
    
    visible = true;
    touchable = false;

    draw->stopAllActions();
    draw->runAction(Sequence::create(
        DelayTime::create(config::getAlertFadeTime() * 0.2),
        EaseInOut::create(ScaleTo::create(config::getAlertFadeTime(), 1, 0), 3),
        CallFunc::create([this]() {
            visible = false;
            Director::getInstance()
                ->getTouchDispatcher()
                ->removeDelegate(this);
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
                MoveTo::create(config::getAlertFadeTime(), pos),
                FadeOut::create(config::getAlertFadeTime()),
                NULL
            ),
            3
        ));
    }

    if (desc) {
        auto pos = Point(getContentSize().width + desc->getContentSize().width, desc->getPositionY());
        desc->stopAllActions();
        desc->runAction(EaseInOut::create(MoveTo::create(config::getAlertFadeTime(), pos), 3));
    }
}

void Alert::enableCloseOnTap(const bool flag)
{
    closeOnTap = flag;
    if (tap) tap->setVisible(flag);
}

void Alert::setDescription(const std::string& description)
{
    if (desc) {
        desc->setString(description.c_str());
        return;
    }

    desc = fonts::createNormal(description.c_str(), 72);
    addChild(desc);

    desc->setAnchorPoint({0.5, 1});
    desc->setPosition(Point(getContentSize() / 2));
    desc->setPositionX(desc->getContentSize().width * -1);
}

bool Alert::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void Alert::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!touchable || !closeOnTap) {
        return;
    }

    if (buttons.empty()) {
        hide();
        return;
    }

    for (auto& pair : buttons) {
        auto btn = pair.first;
        auto cb = pair.second;

        auto location = convertTouchToNodeSpace(touch);
        location.x = buttonContainer->convertToNodeSpace(location).x;

        if (btn->boundingBox().containsPoint(location)) {
            buttonCb = cb;
            hide();
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
    draw->drawPolygon(verts, 4, color::toRGBA(Color3B::BLACK), 0, {});

    auto label = fonts::createLight(description, 36);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setPosition(Point(size / 2));
    node->addChild(label);
}