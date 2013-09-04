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
    updateLabelString();

    setContentSize(label->getContentSize());
    label->setPositionX(label->getContentSize().width);

    return true;
}

void GameTimer::addLabel()
{
    // FIX#4
    label = fonts::createLight("", 36);
    addChild(label);

    label->setColor(Color3B::BLACK);

    label->setAnchorPoint({1, 0});
    label->setHorizontalAlignment(TextHAlignment::RIGHT);
}

void GameTimer::pause()
{
    paused = true;
}

void GameTimer::resume(const float delay)
{
    pauseDelay = delay;
    paused = false;
}

void GameTimer::start()
{
    if (!started) {
        started = true;
        schedule(schedule_selector(GameTimer::onTick), 0.1);
    }
}

void GameTimer::stop()
{
    unschedule(schedule_selector(GameTimer::onTick));
}

bool GameTimer::isStarted() const
{
    return started;
}

void GameTimer::onTick(const float dt)
{
    if (paused) {
        return;
    }

    if (pauseDelay > 0) {
        pauseDelay -= dt;
        return;
    }
    
    time = fmax(0.0, time - dt);
    updateLabelString();

    // detect timeover
    if (time == 0.0) {
        stop();
        page->timeover();
    }

    // update the timer color if required
    if (time <= config::getGameAlarmTime()) {
        auto color = label->getColor();
        color.r = 255 - int(255.0 / config::getGameAlarmTime() * time);
        label->setColor(color);
    }
}

void GameTimer::updateLabelString()
{
    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%.1f", time);
    label->setString(buf);
}