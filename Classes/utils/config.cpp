#include "config.h"

#include "color.h"

using namespace cocos2d;

namespace config {

float getScaleFactor()
{
    return getFrameSize().width / getDesignWidth();
}

cocos2d::Size getFrameSize()
{
    return Director::getInstance()->getOpenGLView()->getFrameSize();
}

float getTouchIgnoreLength()
{
    return 25 * getScaleFactor();
}

float getSwipeMovementMinX()
{
    return 10 * getScaleFactor();
}

float getSwipeMovementMaxY()
{
    return 10 * getScaleFactor();
}

float getQuestionStartDelay()
{
    return getQuestionFadeTime() * 2;
}

float getProgressbarHeight()
{
    return 8 * getScaleFactor();
}

float getAnswerButtonPadding()
{
    return 30 * config::getScaleFactor();
}

cocos2d::Size getAnswerButtonSize()
{
    return {getFrameSize().width, 80 * getScaleFactor()};
}

std::string getGroupHeadline(const int group)
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

cocos2d::Color3B getGroupColor(const int group)
{
    // == 1
    if        (group == 1) {
        return {53, 202, 252};
    } else if (group == 2) {
        return {27, 153, 252};

    // == 2
    } else if (group == 3) {
        return {134, 227, 0};
    } else if (group == 4) {
        return {0, 214, 83};

    // == 3
    } else if (group == 5) {
        return {238, 218, 2};
    } else if (group == 6) {
        return {255, 162, 0};

    // == 4
    } else if (group == 7) {
        return {255, 2, 4};
    } else if (group == 8) {
        return {196, 0, 1};

    // == 5
    } else if (group == 9) {
        return {232, 40, 252};
    } else if (group == 10) {
        return {161, 0, 212};

    // == ?
    } else {
        throw new std::runtime_error("invalid group given");
    }
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