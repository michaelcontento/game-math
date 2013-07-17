#include "GameTimer.h"

#include <algorithm>
#include "utils/fonts.h"
#include "pages/GamePage.h"

using namespace cocos2d;

GameTimer* GameTimer::create(GamePage& page)
{
    GameTimer* pRet = new GameTimer();
    if (pRet && pRet->init(page)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GameTimer::init(GamePage& page)
{
    if (!Node::init()) {
        return false;
    }

    this->page = &page;

    addLabel();

    return true;
}

void GameTimer::addLabel()
{
    // FIX#4
    label = fonts::createLight("", 36);
    updateLabelString();
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setAnchorPoint({1, 1});
    label->setHorizontalAlignment(kTextAlignmentRight);
    label->setVerticalAlignment(kVerticalTextAlignmentTop);
}

void GameTimer::start()
{
    schedule(schedule_selector(GameTimer::onTick), 0.1);
}

void GameTimer::onTick(const float dt)
{
    time = fmax(0.0, time - dt);

    if (time == 0.0) {
        unschedule(schedule_selector(GameTimer::onTick));
        page->timeover();
    }

    if (time <= config::getGameAlarmTime()) {
        auto color = label->getColor();
        color.r = 255 - int(255.0 / config::getGameAlarmTime() * time);
        label->setColor(color);
    }

    updateLabelString();
}

void GameTimer::updateLabelString()
{
    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%.1f", time);
    label->setString(buf);
}