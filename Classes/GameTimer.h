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
    GamePage* page = nullptr;
    float hintTimer = 0;
    HintButton* hint = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    bool started = false;
    bool paused = false;
    float pauseDelay = 0;
    float time = config::getGameTime();

    void addLabel();
    void updateLabelString();
    cocos2d::Sprite* createStar() const;
};

#endif // MATH_GAMETIMER_H
