#ifndef MATH_GAMETIMER_H
#define MATH_GAMETIMER_H

#include <vector>
#include "cocos2d.h"
#include "utils/config.h"

class GamePage;
class HintButton;

class GameTimer : public cocos2d::Node
{
public:
    GameTimer()
    : page(nullptr)
    , hintTimer(0)
    , hint(nullptr)
    , label(nullptr)
    , started(false)
    , paused(false)
    , pauseDelay(0)
    , time(0)
    {
        time = config::getGameTime();
    };

    static GameTimer* create(void) = delete;
    static GameTimer* create(GamePage& page);
    bool init(GamePage& page);

    void start();
    void stop();
    bool isStarted() const;

    void pause();
    void resume(const float delay = 0);

    void onTick(const float dt);
    void setHintButton(HintButton& hint);
    void resetHintTimer();

private:
    GamePage* page;
    float hintTimer;
    HintButton* hint;
    cocos2d::LabelTTF* label;
    bool started;
    bool paused;
    float pauseDelay;
    float time;

    void addLabel();
    void updateLabelString();
    cocos2d::Sprite* createStar() const;
};

#endif // MATH_GAMETIMER_H
