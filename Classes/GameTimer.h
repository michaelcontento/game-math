#ifndef MATH_GAMETIMER_H
#define MATH_GAMETIMER_H

#include "cocos2d.h"
#include "utils/config.h"

class GamePage;

class GameTimer : public cocos2d::Node
{
public:
    static GameTimer* create(void) = delete;
    static GameTimer* create(GamePage& page);
    bool init(GamePage& page);

    void start();
    void onTick(const float dt);

private:
    GamePage* page = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    float time = config::getGameTime();

    void addLabel();
    void updateLabelString();
};

#endif // MATH_GAMETIMER_H
