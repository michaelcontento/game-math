#include "config.h"

#include "color.h"

using namespace cocos2d;

namespace config {

const float getScaleFactor()
{
    return getFrameSize().width / getDesignWidth();
}

const cocos2d::Size& getFrameSize()
{
    return Director::getInstance()->getOpenGLView()->getFrameSize();
}

const float getTouchIgnoreLength()
{
    return 25 * getScaleFactor();
}

const float getSwipeMovementMinX()
{
    return 10 * getScaleFactor();
}

const float getSwipeMovementMaxY()
{
    return 10 * getScaleFactor();
}

const float getQuestionStartDelay()
{
    return getQuestionFadeTime() * 2;
}

const float getProgressbarHeight()
{
    return 8 * getScaleFactor();
}

const float getAnswerButtonPadding()
{
    return 30 * config::getScaleFactor();
}

const cocos2d::Size getAnswerButtonSize()
{
    return {getFrameSize().width, 80 * getScaleFactor()};
}

const std::string getGroupHeadline(const int group)
{
    // == 1
    if        (group == 1) {
        return "ADDITION";
    } else if (group == 2) {
        return "SUBTRACTION";

    // == 2
    } else if (group == 3) {
        return "MULTIPLICATION";
    } else if (group == 4) {
        return "DIVISION";

    // == 3
    } else if (group == 5) {
        return "ARITHMETIC";
    } else if (group == 6) {
        return "FRACTIONS";

    // == 4
    } else if (group == 7) {
        return "POWERS";
    } else if (group == 8) {
        return "STATISTICS";

    // == 5
    } else if (group == 9) {
        return "EQUATIONS";
    } else if (group == 10) {
        return "MIXED";

    // == ?
    } else {
        throw new std::runtime_error("invalid group given");
    }
}

const cocos2d::Color3B getGroupColor(const int group)
{
    cocos2d::extension::HSV hsv = {0, 0.95, 1.00};
    hsv.h += (0.618033988749895 * 360.0) * group;
    hsv.h = fmod(hsv.h, 360.0);
    return color::toRGB(hsv);
}

puzzle::Generator getGenerator(const int group, const int level)
{
    return {
        "{number} {operator} {number}",
        "{number}",
        {puzzle::Operator::PLUS},
        {puzzle::NumberRange::SMALL}
    };
}

} // namespace config