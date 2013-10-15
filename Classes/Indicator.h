#ifndef MATH_INDICATOR_H
#define MATH_INDICATOR_H

#include "cocos2d.h"
#include "utils/config.h"

class Indicator : public cocos2d::NodeRGBA
{
public:
    Indicator()
    : padding(8 * config::getScaleFactor())
    , posY(25 * config::getScaleFactor())
    , opacityActive(0.9)
    , opacityInactive(0.3)
    , nextPosX(0)
    {};

    CREATE_FUNC(Indicator);

    void addDot();
    void updateOpacity();

private:
    const float padding;
    const float posY;
    const float opacityActive;
    const float opacityInactive;
    float nextPosX;
};

#endif // MATH_INDICATOR_H
