#include "Alert.h"

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
    tap = fonts::createLight("tap to continue", 36);
    addChild(tap);

    tap->setAnchorPoint({0.5, 1});
    tap->setPositionX(getContentSize().width + tap->getContentSize().width);
    tap->setPositionY(getContentSize().height / 2 * -1);

    return true;
}

void Alert::show(const std::function<void ()> callback)
{
    this->callback = callback;
    visible = true;
    touchable = false;

    draw->setScaleY(0);
    draw->stopAllActions();
    draw->runAction(EaseInOut::create(ScaleTo::create(config::getAlertFadeTime(), 1, 1), 3));

    tap->stopAllActions();
    tap->runAction(Sequence::create(
        DelayTime::create(config::getAlertFadeTime() * 0.2),
        EaseInOut::create(
            Spawn::create(
                MoveTo::create(config::getAlertFadeTime(), {getContentSize().width / 2, tap->getPositionY()}),
                FadeIn::create(config::getAlertFadeTime()),
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

    if (desc) {
        desc->stopAllActions();
        desc->runAction(Sequence::create(
            DelayTime::create(config::getAlertFadeTime() * 0.2),
            EaseInOut::create(MoveTo::create(config::getAlertFadeTime(), {getContentSize().width / 2, desc->getPositionY()}), 3),
            NULL
        ));
    }
}

void Alert::hide()
{
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
            this->callback();
        }),
        NULL
    ));

    auto pos = Point(tap->getContentSize().width * -1, tap->getPositionY());
    tap->stopAllActions();
    tap->runAction(EaseInOut::create(
        Spawn::create(
            MoveTo::create(config::getAlertFadeTime(), pos),
            FadeOut::create(config::getAlertFadeTime()),
            NULL
        ),
        3
    ));

    if (desc) {
        auto pos = Point(getContentSize().width + desc->getContentSize().width, desc->getPositionY());
        desc->stopAllActions();
        desc->runAction(EaseInOut::create(MoveTo::create(config::getAlertFadeTime(), pos), 3));
    }
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
    desc->setPosition(getContentSize() / 2);
    desc->setPositionX(desc->getContentSize().width * -1);
}

bool Alert::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void Alert::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!touchable) {
        return;
    }
    
    hide();
}

void Alert::visit()
{
    if (visible) {
        Layer::visit();
    }
}