#ifndef MATH_INDICATOR_H
#define MATH_INDICATOR_H

#include "cocos2d.h"
#include "utils/config.h"

class Indicator : public cocos2d::NodeRGBA
{
public:
    CREATE_FUNC(Indicator);

    void addDot();
    void updateOpacity();

private:
    const float padding = 8 * config::getScaleFactor();
    const float posY = 25 * config::getScaleFactor();
    const float opacityActive = 0.9;
    const float opacityInactive = 0.3;
    float nextPosX = 0;
};

#endif // MATH_INDICATOR_H
