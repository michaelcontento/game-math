#include "config.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include <random>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <avalon/platform/android/gnustl_string_fixes.h>
#include "color.h"

using namespace cocos2d;

namespace config {

float getScaleFactor()
{
    static const auto factor = getFrameSize().width / getDesignWidth();
    return factor;
}

float getScaleFactorHeight()
{
    static auto factor = getFrameSize().height / getDesignHeight();
    while (factor >= 2) --factor;
    return factor;
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
    return 0.7;
}

float getProgressbarHeight()
{
    return 8 * std::max(config::getScaleFactor(), config::getScaleFactorHeight());
}

float getAnswerButtonPadding()
{
    return 30 * std::max(config::getScaleFactor(), config::getScaleFactorHeight());
}

cocos2d::Size getAnswerButtonSize()
{
    return {getFrameSize().width, 85 * std::max(config::getScaleFactor(), config::getScaleFactorHeight())};
}

std::string getGroupHeadline(const int group)
{
    if (group > 10 || group < 1) {
        throw new std::runtime_error("invalid group given");
    }

    return _("headlines", std::to_string(group).c_str()).get();
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
        throw new std::range_error("invalid group given");
    }
}

std::function<Question()> getGenerator(const int group, const int level)
{
    int number = ((level - 1) % 8) + 1;
    bool easy = (level <= 8);

    switch (group) {
        case 1:  return getGeneratorAddition(number, easy);
        case 2:  return getGeneratorSubtraction(number, easy);
        case 3:  return getGeneratorMultiplication(number, easy);
        case 4:  return getGeneratorDivision(number, easy);
        case 5:  return getGeneratorArithmetic(number, easy);
        case 6:  return getGeneratorFractions(number, easy);
        case 7:  return getGeneratorPowers(number, easy);
        case 8:  return getGeneratorStatistics(number, easy);
        case 9:  return getGeneratorEquations(number, easy);
        case 10: return getGeneratorMixed(number, easy);
        default: throw new std::range_error("invalid group griven");
    }
}

diceFunction createDice(const int min, const int max)
{
    return [min, max](const std::unordered_set<int> ignore) {
        auto dice = std::bind(
            std::uniform_int_distribution<>{min, max},
            std::mt19937(std::random_device{}())
        );

        int rolls = 0;
        int result = dice();
        while (rolls < 5 && ignore.count(result) > 0) {
            result = dice();
            ++rolls;
        }
        return result;
    };
}

diceFunction createDiceRange(const int pos)
{
    int range = std::max(4, std::min(std::abs(pos) / 2, 17));
    int min = pos - range;
    int max = pos + range;
    if (pos > 0 && min < 0) min = 0;
    return createDice(min, max);
}

std::string formatFraction(const int a, const int b)
{
    return formatFraction(std::to_string(a), std::to_string(b));
};

std::string formatFraction(const std::string a, const std::string b)
{
    return std::string("FRC#") + a + ";" + b + "#";
};

std::string formatPower(const int a)
{
    return std::string("POW#") + std::to_string(a) + "#";
};

std::string formatSqrt(const int a)
{
    return formatSqrt(std::to_string(a));
}

std::string formatSqrt(const std::string a)
{
    return std::string("SQR#") + a + "#";
}

void uniqueValues(const int& a, int& b, int& c)
{
    while (b == a) {
        ++b;
    }
    while (c == a || c == b) {
        ++c;
    }
}

std::function<Question()> getGeneratorAddition(const int number, const bool easy)
{
    switch (number) {
        // Q: 1 + 2
        // A: 3
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(0, 18);
            } else {
                dice = createDice(12, 32);
            }

            int c = dice({0});
            int b = dice({0, c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            int w1 = dice({c});
            int w2 = dice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(a) + " + " + std::to_string(b),
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: ? + 2 = 3
        // A: 1
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(0, 18);
            } else {
                dice = createDice(12, 32);
            }

            int c = dice({0});
            int b = dice({0, c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            auto q = "? + " + std::to_string(b) + " = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                std::swap(a, b);
                q = std::to_string(b) + " + ? = " + std::to_string(c);
            }

            int w1 = dice({a});
            int w2 = dice({a, w1});
            uniqueValues(a, w1, w2);

            return Question(
                q,
                std::to_string(a),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 3
        // A: 1 + 2
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 18);
            } else {
                dice = createDice(10, 28);
            }

            int c = dice({});
            int b = dice({c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            int w1, w2, c1 = 0;
            do {
                c1 = dice({c});
                w1 = dice({c1});
                if (c1 < w1) std::swap(c1, w1);
                w2 = c1 - w1;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                c2 = dice({c});
                w3 = dice({c2});
                if (c2 < w3) std::swap(c2, w3);
                w4 = c2 - w3;
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(c),
                std::to_string(a)  + " + " + std::to_string(b),
                std::to_string(w1) + " + " + std::to_string(w2),
                std::to_string(w3) + " + " + std::to_string(w4)
            );
        };

        // Q: Smallest?
        // A: 1 + 2
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 14);
            } else {
                dice = createDice(10, 24);
            }

            int c = dice({});
            int b = dice({c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            int w1, w2, c1 = 0;
            do {
                c1 = dice({c});
                w1 = dice({c1});
                if (c1 < w1) std::swap(c1, w1);
                w2 = c1 - w1;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                c2 = dice({c});
                w3 = dice({c2});
                if (c2 < w3) std::swap(c2, w3);
                w4 = c2 - w3;
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.front() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.front() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "smallest").get(),
                std::to_string(l1) + " + " + std::to_string(l2),
                std::to_string(l3) + " + " + std::to_string(l4),
                std::to_string(l5) + " + " + std::to_string(l6)
            );
        };

        // Q: 1 + 2 + 3
        // A: 4
        case 5: return [easy]() {
            diceFunction dice;
            int max = 0;
            if (easy) {
                max = 9;
                dice = createDice(1, max);
            } else {
                max = 12;
                dice = createDice(1, max);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});
            int d = a + b + c;

            auto wrongDice = createDice(d / 3, d + max);
            int w1 = wrongDice({d});
            int w2 = wrongDice({d, w1});
            uniqueValues(d, w1, w2);

            return Question(
                std::to_string(a) + " + " + std::to_string(b) + " + " + std::to_string(c),
                std::to_string(d),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1 + 2
        // A: 2 + 0
        case 6: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 16);
            } else {
                dice = createDice(10, 28);
            }

            int c = dice({});
            int b = dice({c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            auto qDice = createDice(0, c);
            int q1 = qDice({a});
            int q2 = c - q1;

            int w1, w2, c1 = 0;
            do {
                c1 = dice({c});
                w1 = dice({c1});
                if (c1 < w1) std::swap(c1, w1);
                w2 = c1 - w1;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                c2 = dice({c});
                w3 = dice({c2});
                if (c2 < w3) std::swap(c2, w3);
                w4 = c2 - w3;
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(q1) + " + " + std::to_string(q2),
                std::to_string(a)  + " + " + std::to_string(b),
                std::to_string(w1) + " + " + std::to_string(w2),
                std::to_string(w3) + " + " + std::to_string(w4)
            );
        };

        // Q: Largest?
        // A: 1 + 1
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 14);
            } else {
                dice = createDice(10, 24);
            }

            int c = dice({});
            int b = dice({c});
            if (c < b) std::swap(b, c);
            int a = c - b;

            int w1, w2, c1 = 0;
            do {
                c1 = dice({c});
                w1 = dice({c1});
                if (c1 < w1) std::swap(c1, w1);
                w2 = c1 - w1;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                c2 = dice({c});
                w3 = dice({c2});
                if (c2 < w3) std::swap(c2, w3);
                w4 = c2 - w3;
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.back() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.back() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "largest").get(),
                std::to_string(l1) + " + " + std::to_string(l2),
                std::to_string(l3) + " + " + std::to_string(l4),
                std::to_string(l5) + " + " + std::to_string(l6)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorAddition(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorSubtraction(const int number, const bool easy)
{
    switch (number) {
        // Q: 1 - 2
        // A: 3
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(5, 14);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            int w1 = dice({c});
            int w2 = dice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(a) + " - " + std::to_string(b),
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: ? - 2 = 3
        // A: 5
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(5, 14);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            auto q = "? - " + std::to_string(b) + " = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                std::swap(a, b);
                q = std::to_string(b) + " - ? = " + std::to_string(c);
            }

            int w1 = dice({a});
            int w2 = dice({a, w1});
            uniqueValues(a, w1, w2);

            return Question(
                q,
                std::to_string(a),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 3
        // A: 1 - 2
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(5, 14);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 + w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 + w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(c),
                std::to_string(a)  + " - " + std::to_string(b),
                std::to_string(w1) + " - " + std::to_string(w2),
                std::to_string(w3) + " - " + std::to_string(w4)
            );
        };

        // Q: Smallest?
        // A: 1 - 2
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 6);
            } else {
                dice = createDice(5, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 + w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 + w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.front() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.front() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "smallest").get(),
                std::to_string(l1) + " - " + std::to_string(l2),
                std::to_string(l3) + " - " + std::to_string(l4),
                std::to_string(l5) + " - " + std::to_string(l6)
            );
        };

        // Q: 1 - 2 - 3
        // A: 4
        case 5: return [easy]() {
            diceFunction dice;
            int max = 0;
            if (easy) {
                max = 9;
                dice = createDice(1, max);
            } else {
                max = 12;
                dice = createDice(1, max);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});
            int d = a + b + c;
            std::swap(d, a);

            auto wrongDice = createDice(d / 3, d + max);
            int w1 = wrongDice({d});
            int w2 = wrongDice({d, w1});
            uniqueValues(d, w1, w2);

            return Question(
                std::to_string(a) + " - " + std::to_string(b) + " - " + std::to_string(c),
                std::to_string(d),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1 - 2
        // A: 2 - 0
        case 6: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 6);
            } else {
                dice = createDice(5, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            int q2 = dice({b});
            int q1 = c + q2;

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 + w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 + w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(q1) + " - " + std::to_string(q2),
                std::to_string(a)  + " - " + std::to_string(b),
                std::to_string(w1) + " - " + std::to_string(w2),
                std::to_string(w3) + " - " + std::to_string(w4)
            );
        };

        // Q: Largest?
        // A: 1 - 1
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 6);
            } else {
                dice = createDice(5, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = a + b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 + w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 + w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.back() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.back() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "largest").get(),
                std::to_string(l1) + " - " + std::to_string(l2),
                std::to_string(l3) + " - " + std::to_string(l4),
                std::to_string(l5) + " - " + std::to_string(l6)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorSubtraction(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorMultiplication(const int number, const bool easy)
{
    switch (number) {
        // Q: 1 x 2
        // A: 3
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;

            auto qDice = createDiceRange(c);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(a) + " * " + std::to_string(b),
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: ? x 2 = 3
        // A: 5
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;

            auto q = "? x " + std::to_string(b) + " = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                std::swap(a, b);
                q = std::to_string(b) + " x ? = " + std::to_string(c);
            }

            int w1 = dice({a});
            int w2 = dice({a, w1});
            uniqueValues(a, w1, w2);

            return Question(
                q,
                std::to_string(a),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 3
        // A: 1 x 2
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 * w2;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 * w4;
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(c),
                std::to_string(a)  + " * " + std::to_string(b),
                std::to_string(w1) + " * " + std::to_string(w2),
                std::to_string(w3) + " * " + std::to_string(w4)
            );
        };

        // Q: Smallest?
        // A: 1 x 2
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 * w2;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 * w4;
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.front() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.front() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "smallest").get(),
                std::to_string(l1) + " * " + std::to_string(l2),
                std::to_string(l3) + " * " + std::to_string(l4),
                std::to_string(l5) + " * " + std::to_string(l6)
            );
        };

        // Q: 1 x 2 x 3
        // A: 4
        case 5: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 7);
            } else {
                dice = createDice(2, 10);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});
            int d = a * b * c;

            auto qDice = createDiceRange(d);
            int w1 = qDice({d});
            int w2 = qDice({d, w1});
            uniqueValues(d, w1, w2);

            return Question(
                std::to_string(a) + " * " + std::to_string(b) + " * " + std::to_string(c),
                std::to_string(d),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1 x 2
        // A: 2 x 0
        case 6: return [easy]() {
            diceFunction dice;
            int max;
            if (easy) {
                max = 10;
                dice = createDice(1, max);
            } else {
                max = 16;
                dice = createDice(4, max);
            }

            auto isPrime = [](const int test) {
                if (test <= 1) {
                    return false;
                }

                for (int i = 2; i < test; i++) {
                    if (test % i==0) {
                        return false;
                    }
                }

                return true;
            };

            int a, b, c = 0;
            do {
                a = dice({});
                b = dice({a});
                c = a * b;
            } while (isPrime(c));

            int q1, q2 = 0;
            bool found = false;
            for (float i = 2; i <= max; ++i) {
                if (i == a || i == b) {
                    continue;
                }

                float res = c / i;
                if (static_cast<float>(static_cast<int>(res)) == res) {
                    q1 = i;
                    q2 = res;
                    found = true;
                    break;
                }
            }
            if (!found) {
                q1 = b;
                q2 = a;
            }

            std::swap(a, q1);
            std::swap(b, q2);

            int w1, w2, c1 = 0;
            do {
                w1 = dice({a, b, q1, q2});
                w2 = dice({w1});
                c1 = w1 * w2;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w3 = dice({a, b, q1, q2, w1, w2});
                w4 = dice({w3});
                c2 = w3 * w4;
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(q1) + " * " + std::to_string(q2),
                std::to_string(a)  + " * " + std::to_string(b),
                std::to_string(w1) + " * " + std::to_string(w2),
                std::to_string(w3) + " * " + std::to_string(w4)
            );
        };

        // Q: Largest?
        // A: 1 x 1
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 * w2;
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 * w4;
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.back() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.back() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "largest").get(),
                std::to_string(l1) + " * " + std::to_string(l2),
                std::to_string(l3) + " * " + std::to_string(l4),
                std::to_string(l5) + " * " + std::to_string(l6)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorMultiplication(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorDivision(const int number, const bool easy)
{
    switch (number) {
        // Q: 2 / 1
        // A: 2
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;
            std::swap(c, a);

            auto qDice = createDiceRange(c);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(a) + " / " + std::to_string(b),
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: ? / 2 = 5
        // A: 10
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;
            std::swap(c, a);

            auto q = "? / " + std::to_string(b) + " = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                std::swap(a, b);
                q = std::to_string(b) + " / ? = " + std::to_string(c);
            }

            auto qDice = createDiceRange(a);
            int w1 = qDice({a});
            int w2 = qDice({a, w1});
            uniqueValues(a, w1, w2);

            return Question(
                q,
                std::to_string(a),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 3
        // A: 3 / 1
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w1 = dice({b});
                w2 = dice({w1});
                c1 = w1 * w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w3 = dice({b, w1});
                w4 = dice({w3});
                c2 = w3 * w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(c),
                std::to_string(a)  + " / " + std::to_string(b),
                std::to_string(w1) + " / " + std::to_string(w2),
                std::to_string(w3) + " / " + std::to_string(w4)
            );
        };

        // Q: Smallest?
        // A: 2 / 2
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 * w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 * w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.front() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.front() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "smallest").get(),
                std::to_string(l1) + " / " + std::to_string(l2),
                std::to_string(l3) + " / " + std::to_string(l4),
                std::to_string(l5) + " / " + std::to_string(l6)
            );
        };

        // Q: 24 / 2 / 6
        // A: 2
        case 5: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 7);
            } else {
                dice = createDice(2, 10);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});
            int d = a * b * c;
            std::swap(d, a);

            auto qDice = createDiceRange(d);
            int w1 = qDice({d});
            int w2 = qDice({d, w1});
            uniqueValues(d, w1, w2);

            return Question(
                std::to_string(a) + " / " + std::to_string(b) + " / " + std::to_string(c),
                std::to_string(d),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 2 / 1
        // A: 1 / 1
        case 6: return [easy]() {
            diceFunction dice;
            int max;
            if (easy) {
                max = 10;
                dice = createDice(2, max);
            } else {
                max = 16;
                dice = createDice(4, max);
            }

            int a, b, c = 0;
            a = dice({});
            b = dice({a});
            c = a * b;
            std::swap(c, a);

            int q2 = dice({b});
            int q1 = c * q2;

            std::swap(a, q1);
            std::swap(b, q2);

            int w1, w2, c1 = 0;
            do {
                w1 = dice({a, b, q1, q2});
                w2 = dice({w1});
                c1 = w1 * w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w3 = dice({a, b, q1, q2, w1, w2});
                w4 = dice({w3});
                c2 = w3 * w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            return Question(
                std::to_string(q1) + " / " + std::to_string(q2),
                std::to_string(a)  + " / " + std::to_string(b),
                std::to_string(w1) + " / " + std::to_string(w2),
                std::to_string(w3) + " / " + std::to_string(w4)
            );
        };

        // Q: Largest?
        // A: 10 / 1
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int a = dice({});
            int b = dice({a});
            int c = a * b;
            std::swap(c, a);

            int w1, w2, c1 = 0;
            do {
                w2 = dice({b});
                w1 = dice({w2});
                c1 = w1 * w2;
                std::swap(c1, w1);
            } while (c1 == c);

            int w3, w4, c2 = 0;
            do {
                w4 = dice({b, w2});
                w3 = dice({w4});
                c2 = w3 * w4;
                std::swap(c2, w3);
            } while (c2 == c || c2 == c1);

            std::vector<int> sort {c, c1, c2};
            std::sort(sort.begin(), sort.end());

            int l1, l2, l3, l4, l5, l6 = 0;
            if (sort.back() == c) {
                l1 = a;
                l2 = b;
                l3 = w1;
                l4 = w2;
                l5 = w3;
                l6 = w4;
            } else if (sort.back() == c1)  {
                l1 = w1;
                l2 = w2;
                l3 = a;
                l4 = b;
                l5 = w3;
                l6 = w4;
            } else {
                l1 = w3;
                l2 = w4;
                l3 = a;
                l4 = b;
                l5 = w1;
                l6 = w2;
            }

            return Question(
                _("questions", "largest").get(),
                std::to_string(l1) + " / " + std::to_string(l2),
                std::to_string(l3) + " / " + std::to_string(l4),
                std::to_string(l5) + " / " + std::to_string(l6)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorDivision(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorPowers(const int number, const bool easy)
{
    switch (number) {
        // Q: 2 x 2 x 2
        // A: 2^3
        case 1:  return [easy]() {
            diceFunction diceBase, diceExp;
            if (easy) {
                diceBase = createDice(1, 8);
                diceExp = createDice(1, 3);
            } else {
                diceBase = createDice(1, 9);
                diceExp = createDice(1, 5);
            }

            int base = diceBase({});
            int exp = diceExp({});

            auto wDice = createDiceRange(exp);
            int w1 = wDice({exp});
            int w2 = wDice({exp, w1});
            uniqueValues(exp, w1, w2);

            std::string q = std::to_string(base);
            for (int i = 1; i < exp; ++i) {
                q += " * " + std::to_string(base);
            }

            std::string prefix = std::to_string(base);
            return Question(
                q,
                prefix + formatPower(exp),
                prefix + formatPower(w1),
                prefix + formatPower(w2)
            );
        };

        // Q: 4^2
        // A: 16
        case 2: return [easy]() {
            diceFunction diceBase, diceExp;
            int max;
            if (easy) {
                diceBase = createDice(1, 6);
                diceExp = createDice(1, 3);
                max = 100;
            } else {
                diceBase = createDice(3, 12);
                diceExp = createDice(1, 4);
                max = 300;
            }

            int base, exp, res;
            do {
                base = diceBase({});
                exp = diceExp({});
                res = std::pow(base, exp);
            } while (res > max);

            auto wDice = createDiceRange(res);
            int w1 = wDice({res});
            int w2 = wDice({res, w1});
            uniqueValues(res, w1, w2);

            return Question(
                std::to_string(base) + formatPower(exp),
                std::to_string(res),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: sqrt(16)
        // A: 4
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 12);
            } else {
                dice = createDice(4, 20);
            }

            int base = dice({});
            int res = base * base;

            auto wDice = createDiceRange(base);
            int w1 = wDice({base});
            int w2 = wDice({base, w1});
            uniqueValues(base, w1, w2);

            return Question(
                formatSqrt(res),
                std::to_string(base),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: (a + b)^2
        // A: 4
        case 4: return [easy]() {
            diceFunction diceBase, diceExp;
            if (easy) {
                diceBase = createDice(1, 9);
                diceExp = createDice(2, 2);
            } else {
                diceBase = createDice(3, 12);
                diceExp = createDice(2, 3);
            }

            int base, exp, res;
            do {
                base = diceBase({});
                exp = diceExp({});
                res = std::pow(base, exp);
            } while (res > 200);


            int a = createDice(1, base)({});
            int b = base - a;
            std::string op = "+";
            if (createDice(0, 1)({}) == 0) {
                a = createDice(base, base * 3)({base});
                b = a - base;
                op = "-";
            }

            auto wDice = createDiceRange(res);
            int w1 = wDice({res});
            int w2 = wDice({res, w1});
            uniqueValues(res, w1, w2);

            return Question(
                "(" + std::to_string(a) + " " + op + " " + std::to_string(b) + ")" + formatPower(exp),
                std::to_string(res),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: sqrt(a + b)
        // A: 3
        case 5: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 12);
            } else {
                dice = createDice(4, 20);
            }

            int base = dice({});
            int res = base * base;

            int a = createDice(1, res)({});
            int b = res - a;
            std::string op = "+";
            if (createDice(0, 1)({}) == 0) {
                a = createDice(1, 100)({}) + res;
                b = a - res;
                op = "-";
            }

            auto wDice = createDiceRange(base);
            int w1 = wDice({base});
            int w2 = wDice({base, w1});
            uniqueValues(base, w1, w2);

            return Question(
                formatSqrt(std::to_string(a) + " " + op + " " + std::to_string(b)),
                std::to_string(base),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1^8 * 1^2
        // A: 1^10
        case 6: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 9);
            } else {
                dice = createDice(4, 16);
            }

            int base = dice({});
            int expA = dice({});
            int expB = dice({});
            int sum = expA + expB;

            auto qDice = createDiceRange(sum);
            int w1 = qDice({sum});
            int w2 = qDice({sum, w1});
            uniqueValues(sum, w1, w2);

            return Question(
                std::to_string(base) + formatPower(expA) + " * " + std::to_string(base) + formatPower(expB),
                std::to_string(base) + formatPower(sum),
                std::to_string(base) + formatPower(w1),
                std::to_string(base) + formatPower(w2)
            );
        };

        // Q: 16
        // A: 4^2
        case 7: return [easy] {
            diceFunction diceBase, diceExp;
            int max;
            if (easy) {
                diceBase = createDice(1, 6);
                diceExp = createDice(1, 3);
                max = 100;
            } else {
                diceBase = createDice(3, 12);
                diceExp = createDice(1, 4);
                max = 300;
            }

            int base, exp, res;
            do {
                base = diceBase({});
                exp = diceExp({});
                res = std::pow(base, exp);
            } while (res > max);

            auto wDiceBase = createDiceRange(base);
            auto wDiceExp = createDiceRange(exp);

            int w1, w2, res2;
            do {
                w1 = wDiceBase({base});
                w2 = wDiceExp({exp});
                res2 = std::pow(w1, w2);
            } while (res2 == res);

            int w3, w4, res3;
            do {
                w3 = wDiceBase({base});
                w4 = wDiceExp({exp});
                res3 = std::pow(w3, w4);
            } while (res3 == res || res3 == res2);

            return Question(
                std::to_string(res),
                std::to_string(base) + formatPower(exp),
                std::to_string(w1)   + formatPower(w2),
                std::to_string(w3)   + formatPower(w4)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorPowers(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorFractions(const int number, const bool easy)
{
    switch (number) {
        // Q: 3/2 x 3
        // A: 9/2
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});

            int a1 = a * c;
            int a2 = a * b;
            int a3 = b * c;
            uniqueValues(a1, a2, a3);

            return Question(
                formatFraction(a, b) + " * " + std::to_string(c),
                formatFraction(a1,  b),
                formatFraction(a2,  b),
                formatFraction(a3,  a)
            );
        };

        // Q: 8/9 / 7
        // A: 8/63
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});

            int b1 = b * c;
            int b2 = c * a;
            int b3 = a * b;
            uniqueValues(b1, b2, b3);

            return Question(
                formatFraction(a, b) + " / " + std::to_string(c),
                formatFraction(a, b1),
                formatFraction(a, b2),
                formatFraction(c, b3)
            );
        };

        // Q: 4/5 + 8
        // A: 44/5
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});

            int a1 = a + (b * c);
            int a2 = a + (a * c);
            int a3 = a - (b * c);
            uniqueValues(a1, a2, a3);

            return Question(
                formatFraction(a, b) + " + " + std::to_string(c),
                formatFraction(a1, b),
                formatFraction(a2, b),
                formatFraction(a3, a)
            );
        };

        // Q: 22/6 - 2
        // A: 10/6
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a, b});

            int a1 = a;
            int a2 = c;
            int a3 = (a + c) / 2;
            uniqueValues(a1, a2, a3);

            return Question(
                formatFraction(a + (b * c), b) + " - " + std::to_string(c),
                formatFraction(a1, b),
                formatFraction(a2, b),
                formatFraction(a3, a)
            );
        };

        // Q: 1/2 * 3/4
        // A: 3/8
        case 5: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a});
            int d = dice({b, c});

            int a1 = a * c;
            int b1 = b * d;
            int a2 = a * d;
            int b2 = b * c;
            int a3 = b * c;
            int b3 = a * b;
            uniqueValues(b1, b2, b3);

            return Question(
                formatFraction(a, b) + " * " + formatFraction(c, d),
                formatFraction(a1, b1),
                formatFraction(a2, b2),
                formatFraction(a3, b3)
            );
        };

        // Q: 5/3 / 7/5
        // A: 25/21
        case 6: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({a});
            int d = dice({b, c});

            int a1 = a * d;
            int b1 = b * c;
            int a2 = a * c;
            int b2 = b * d;
            int a3 = b * d;
            int b3 = a * b;
            uniqueValues(b1, b2, b3);

            return Question(
                formatFraction(a, b) + " / " + formatFraction(c, d),
                formatFraction(a1, b1),
                formatFraction(a2, b2),
                formatFraction(a3, b3)
            );
        };

        // Q: 4/8 = ?
        // A: 1/2
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(2, 8);
            } else {
                dice = createDice(3, 12);
            }

            int a = dice({});
            int b = dice({a});
            int c = dice({});

            auto aDice = createDiceRange(a);
            auto bDice = createDiceRange(b);

            int a2 = aDice({0, a});
            int b2 = aDice({0, b, a2});

            int a3 = aDice({0, a, a2});
            int b3 = aDice({0, b, a3, b2});

            uniqueValues(b, b2, b3);

            return Question(
                formatFraction(a * c, b * c) + " = ?",
                formatFraction(a, b),
                formatFraction(a2, b2),
                formatFraction(a3, b3)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorFractions(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorMixed(const int number, const bool easy)
{
    int level = number;
    if (!easy) {
        level += 8;
    }

    return [level]() {
        int group = createDice(1, 9)({9});
        return getGenerator(group, level)();
    };
}

std::function<Question()> getGeneratorArithmetic(const int number, const bool easy)
{
    switch (number) {
        // Q: 10 *ADD/SUB* (-7)
        // A: 2
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});

            if (a > 0 && b > 0) {
                a = a * -1;
                if (createDice(0, 1)({}) == 0) {
                    std::swap(a, b);
                }
            }

            std::string op = "+";
            int c = a + b;
            if (createDice(0, 1)({}) == 0) {
                op = "-";
                c = a - b;
            }

            auto qDice = createDiceRange(c);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});
            uniqueValues(c, w1, w2);

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) {
                aStr = "(" + aStr + ")";
            }
            if (b < 0) {
                bStr = "(" + bStr + ")";
            }

            return Question(
                aStr + " " + op + " " + bStr,
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 10 *MUL/DIV* (-7)
        // A: 2
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});

            if (a > 0 && b > 0) {
                a = a * -1;
                if (createDice(0, 1)({}) == 0) {
                    std::swap(a, b);
                }
            }

            std::string op = "*";
            int c = a * b;
            if (createDice(0, 1)({}) == 0) {
                op = "/";
                std::swap(c, a);
            }

            auto qDice = createDiceRange(c);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});
            uniqueValues(c, w1, w2);

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) {
                aStr = "(" + aStr + ")";
            }
            if (b < 0) {
                bStr = "(" + bStr + ")";
            }

            return Question(
                aStr + " " + op + " " + bStr,
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 2 *MUL/ADD* 81 *ADD/SUB* 9
        // A: 11
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0, });
            int b = dice({0, a});
            int c = dice({0, a, b});

            if (a > 0 && b > 0 && c > 0) {
                a = a * -1;
                if (createDice(0, 1)({}) == 0) {
                    std::swap(a, b);
                } else if (createDice(0, 1)({}) == 0) {
                    std::swap(a, c);
                }
            }

            std::string op1, op2;
            int d = 0;
            int tmpMul = b * c;
            switch (createDice(1, 4)({})) {
                case 1: op1 = "+"; op2 = "*";                       d = a + tmpMul; break;
                case 3: op1 = "-"; op2 = "*";                       d = a - tmpMul; break;
                case 2: op1 = "+"; op2 = "/"; std::swap(tmpMul, b); d = a + tmpMul; break;
                case 4: op1 = "-"; op2 = "/"; std::swap(tmpMul, b); d = a - tmpMul; break;
            }

            auto qDice = createDiceRange(d);
            int w1 = qDice({d});
            int w2 = qDice({d, w1});
            uniqueValues(d, w1, w2);

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            std::string cStr = std::to_string(c);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";
            if (c < 0) cStr = "(" + cStr + ")";

            return Question(
                aStr + " " + op1 + " " + bStr + " " + op2 + " " + cStr,
                std::to_string(d),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 4 *ANY* 3 = 1
        // A: -
        case 4: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});

            std::string op;
            int c = 0;
            switch (createDice(1, 4)({})) {
                case 1: op = "+"; c = a + b; break;
                case 3: op = "-"; c = a - b; break;
                case 2: op = "*"; c = a * b; break;
                case 4: op = "/"; c = a * b; std::swap(c, a); break;
            }

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";

            std::vector<std::string> ops {};
            if (op != "+") ops.push_back("+");
            if (op != "-") ops.push_back("-");
            if (op != "*") ops.push_back("*");
            if (op != "/") ops.push_back("/");
            std::random_shuffle(ops.begin(), ops.end());

            return Question(
                aStr + " ? " + bStr + " = " + std::to_string(c),
                op,
                ops.front(),
                ops.back()
            );
        };

        // Q: 8
        // A: 12 *ANY* 4
        case 5: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});

            std::string op;
            int c = 0;
            switch (createDice(1, 4)({})) {
                case 1: op = "+"; c = a + b; break;
                case 3: op = "-"; c = a - b; break;
                case 2: op = "*"; c = a * b; break;
                case 4: op = "/"; c = a * b; std::swap(c, a); break;
            }

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";

            std::vector<std::string> ops {};
            if (op != "+") ops.push_back("+");
            if (op != "-") ops.push_back("-");
            if (op != "*") ops.push_back("*");
            if (op != "/") ops.push_back("/");
            std::random_shuffle(ops.begin(), ops.end());

            return Question(
                std::to_string(c),
                std::to_string(a) + " " + op          + " " + std::to_string(b),
                std::to_string(a) + " " + ops.front() + " " + std::to_string(b),
                std::to_string(a) + " " + ops.back()  + " " + std::to_string(b)
            );
        };

        // Q: Largest?
        // A: 12 *ANY* 2
        case 6: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a, b;
            float cAdd, cSub, cMul, cDiv;
            do {
                a = dice({0});
                b = dice({0, a});

                cAdd = a + b;
                cSub = a - b;
                cMul = a * b;
                cDiv = a / b;

                std::unordered_set<float> values {cAdd, cSub, cMul, cDiv};
                if (values.size() == 4) break;
            } while (true);

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";

            std::deque<float> ops {cAdd, cSub, cMul, cDiv};
            std::sort(ops.begin(), ops.end());

            std::string mode = _("questions", "largest").get();
            bool largest = true;
            if (createDice(0, 1)({}) == 0) {
                mode = _("questions", "smallest").get();
                largest = false;
            }

            auto getOp = [largest, &ops, cAdd, cSub, cMul, cDiv]() {
                float c = 0;
                if (largest) {
                    c = ops.back();
                    ops.pop_back();
                } else {
                    c = ops.front();
                    ops.pop_front();
                }
                if (c == cAdd) return std::string("+");
                if (c == cSub) return std::string("-");
                if (c == cMul) return std::string("*");
                if (c == cDiv) return std::string("/");
                throw new std::range_error("unable to detect op!");
            };

            return Question(
                mode,
                std::to_string(a) + " " + getOp() + " " + std::to_string(b),
                std::to_string(a) + " " + getOp() + " " + std::to_string(b),
                std::to_string(a) + " " + getOp() + " " + std::to_string(b)
            );
        };

        // Q: 35 *ANY* 3
        // A: 16 *ANY* 2
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 17);
            } else {
                dice = createDice(5, 27);
            }
            std::unordered_set<int> primes {13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

            std::vector<std::string> ops {"+", "-", "/", "*"};
            std::random_shuffle(ops.begin(), ops.end());
            auto op1 = ops.back(); ops.pop_back();
            auto op2 = ops.back(); ops.pop_back();
            auto op3 = ops.back(); ops.pop_back();
            auto op4 = ops.back(); ops.pop_back();

            int c1 = dice(primes);
            int c2 = c1;
            int c3, c4;
            do {
                c3 = dice(primes);
            } while (c3 == c1);
            do {
                c4 = dice(primes);
            } while (c4 == c1);

            auto getEq = [](int c, std::string op) {
                int a, b;
                if (op == "+") {
                    a = createDice(-20, c - 1)({});
                    b = c - a;
                } else if (op == "-") {
                    a = createDice(-20, c - 1)({});
                    b = a - c;
                } else if (op == "/") {
                    b = createDice(1, 9)({});
                    a = c * b;
                } else if (op == "*") {
                    do {
                        b = createDice(1, 9)({});
                        float res = c / static_cast<float>(b);
                        if (static_cast<float>(static_cast<int>(res)) == res) {
                            a = res;
                            break;
                        }
                    } while (true);
                }

                std::string aStr = std::to_string(a);
                std::string bStr = std::to_string(b);
                if (a < 0) aStr = "(" + aStr + ")";
                if (b < 0) bStr = "(" + bStr + ")";

                return aStr + " " + op + " " + bStr;
            };

            return Question(
                getEq(c1, op1),
                getEq(c2, op2),
                getEq(c3, op3),
                getEq(c4, op4)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorArithmetic(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorStatistics(const int number, const bool easy)
{
    switch (number) {
        // Q: 40% of 50
        // A: 20
        case 1:  return [easy]() {
            diceFunction percDiceA, valDice;
            if (easy) {
                percDiceA = createDice(1, 5);
                valDice = createDice(1, 10);
            } else {
                percDiceA = createDice(1, 9);
                valDice = createDice(3, 14);
            }
            auto percDiceB = [easy]() {
                if (easy) return 0;
                return createDice(0, 1)({}) == 0 ? 0 : 5;
            };

            int perc, val, c;
            float tmpC;
            do {
                perc = (percDiceA({}) * 10) + percDiceB();
                val = valDice({}) * 10;
                tmpC = (val / 100.f) * perc;
                c = static_cast<int>(tmpC);
            } while(tmpC != static_cast<float>(c));

            auto wDice = createDiceRange(c);
            int w1 = wDice({c});
            int w2 = wDice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(perc) + "% of " + std::to_string(val),
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1/4
        // A: 25%
        case 2: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 8);
            } else {
                dice = createDice(3, 15);
            }

            int a, b, c;
            float tmpC;
            do {
                a = dice({});
                b = dice({});
                if (!easy && a == b) continue;
                if (a > b) std::swap(a, b);
                tmpC = 100.f / b * a;
                c = static_cast<int>(tmpC);
            } while(tmpC != static_cast<float>(c));

            auto wDice = createDice(1, 5);
            int w1, w2;
            do {
                int tmp = wDice({}) * 5;
                w1 = c - tmp;
                if (w1 < 0) w1 = c + tmp;
            } while (false);
            do {
                int tmp = wDice({}) * 5;
                w2 = c + tmp;
                if (w2 > 100) w2 = c - tmp;
            } while (w2 == w1);

            uniqueValues(c, w1, w2);

            return Question(
                formatFraction(a, b),
                std::to_string(c)  + "%",
                std::to_string(w1) + "%",
                std::to_string(w2) + "%"
            );
        };

        // Q: 3/? = 75%
        // A: 4
        case 3: return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(1, 8);
            } else {
                dice = createDice(3, 15);
            }

            int a, b, c;
            float tmpC;
            do {
                a = dice({});
                b = dice({});
                if (!easy && a == b) continue;
                if (a > b) std::swap(a, b);
                tmpC = 100.f / b * a;
                c = static_cast<int>(tmpC);
            } while(tmpC != static_cast<float>(c));

            std::string q = formatFraction(std::to_string(a), "?") + " = " + std::to_string(c) + "%";
            if (createDice(0, 1)({}) == 0) {
                q = formatFraction("?", std::to_string(b)) + " = " + std::to_string(c) + "%";
                std::swap(a, b);
            }

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                q,
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 80 decreased/increased by 20%
        // A: 64
        case 4: return [easy]() {
            diceFunction percDiceA, valDice;
            if (easy) {
                percDiceA = createDice(1, 5);
                valDice = createDice(1, 10);
            } else {
                percDiceA = createDice(1, 9);
                valDice = createDice(3, 14);
            }
            auto percDiceB = [easy]() {
                if (easy) return 0;
                return createDice(0, 1)({}) == 0 ? 0 : 5;
            };

            int perc, val, c;
            float tmpC;
            do {
                perc = (percDiceA({}) * 10) + percDiceB();
                val = valDice({}) * 10;
                tmpC = (val / 100.f) * perc;
                c = static_cast<int>(tmpC);
            } while(tmpC != static_cast<float>(c));

            std::string mode;
            if (createDice(0, 1)({}) == 0) {
                mode = _("questions", "decreased").get();
                c = val - c;
            } else {
                mode = _("questions", "increased").get();
                c = val + c;
            }

            auto wDice = createDiceRange(c);
            int w1 = wDice({c});
            int w2 = wDice({c, w1});
            uniqueValues(c, w1, w2);

            return Question(
                std::to_string(val) + "\n" + mode + "\n" + std::to_string(perc) + "%",
                std::to_string(c),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 6 is 20% of?
        // A: 30
        case 5: return [easy]() {
            diceFunction percDiceA, valDice;
            if (easy) {
                percDiceA = createDice(1, 5);
                valDice = createDice(1, 10);
            } else {
                percDiceA = createDice(1, 9);
                valDice = createDice(3, 14);
            }
            auto percDiceB = [easy]() {
                if (easy) return 0;
                return createDice(0, 1)({}) == 0 ? 0 : 5;
            };

            int perc, val, c;
            float tmpC;
            do {
                perc = (percDiceA({}) * 10) + percDiceB();
                val = valDice({}) * 10;
                tmpC = (val / 100.f) * perc;
                c = static_cast<int>(tmpC);
            } while(tmpC != static_cast<float>(c));

            auto wDice = createDiceRange(val);
            int w1 = wDice({val});
            int w2 = wDice({val, w1});
            uniqueValues(val, w1, w2);

            return Question(
                _("questions", "percentof")
                    .assign("value", std::to_string(c).c_str())
                    .assign("perc", std::to_string(perc).c_str())
                    .get(),
                std::to_string(val),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 99, 12, 49, 77, 57 Median?
        // A: 57
        case 6: return [easy]() {
            diceFunction nbrDice;
            int amount, medianPos;
            if (easy) {
                amount = 3;
                medianPos = 2;
                nbrDice = createDice(1, 45);
            } else {
                amount = 5;
                medianPos = 3;
                nbrDice = createDice(10, 99);
            }

            int median, lastNbr;
            std::unordered_set<int> usedNbrs {};
            for (int i = 1; i <= amount; ++i) {
                lastNbr = nbrDice(usedNbrs);
                usedNbrs.insert(lastNbr);
                if (i == medianPos) median = lastNbr;
            }
            std::vector<int> shuffelNbr (usedNbrs.begin(), usedNbrs.end());

            std::sort(shuffelNbr.begin(), shuffelNbr.end());
            int pos = 1;
            BOOST_FOREACH (auto& nbr, shuffelNbr) {
                if (pos == medianPos) {
                    median = nbr;
                    break;
                }
                ++pos;
            }

            std::random_shuffle(shuffelNbr.begin(), shuffelNbr.end());
            std::string q;
            BOOST_FOREACH (auto& nbr, shuffelNbr) {
                q += std::to_string(nbr) + ", ";
            }
            q = q.substr(0, q.length() - 2) + "\n" + _("questions", "median").get() + "?";

            int w1, w2;
            do {
                w1 = shuffelNbr.back();
                shuffelNbr.pop_back();
            } while (w1 == median);
            do {
                w2 = shuffelNbr.back();
                shuffelNbr.pop_back();
            } while (w2 == median);

            return Question(
                q,
                std::to_string(median),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 1, 3, 5 Average?
        // A: 3
        case 7: return [easy] {
            int amount = easy ? 3 : 5;

            std::unordered_set<int> usedNbrs;
            auto dice = createDice(1, 70 / amount);
            while (usedNbrs.size() < amount) {
                usedNbrs.insert(dice({}));
            }

            int sum = 0;
            BOOST_FOREACH (auto& nbr, usedNbrs) {
                sum += nbr;
            }

            float avg = sum / static_cast<float>(amount);
            int diff = 0;
            while (static_cast<float>(static_cast<int>(avg)) != avg) {
                ++sum;
                ++diff;
                avg = sum / static_cast<float>(amount);
            }
            int avgInt = static_cast<int>(avg);

            std::vector<int> shuffelNbr (usedNbrs.begin(), usedNbrs.end());
            if (diff > 0) {
                int tmp = shuffelNbr.back();
                shuffelNbr.pop_back();
                shuffelNbr.push_back(tmp + diff);
            }

            std::random_shuffle(shuffelNbr.begin(), shuffelNbr.end());
            std::string q;
            BOOST_FOREACH (auto& nbr, shuffelNbr) {
                q += std::to_string(nbr) + ", ";
            }
            q = q.substr(0, q.length() - 2) + "\n" + _("questions", "average").get() + "?";

            auto wDice = createDiceRange(avg);
            int w1 = wDice({avgInt});
            int w2 = wDice({avgInt, w1});
            uniqueValues(avgInt, w1, w2);

            return Question(
                q,
                std::to_string(avgInt),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorStatistics(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

std::function<Question()> getGeneratorEquations(const int number, const bool easy)
{
    switch (number) {
        // Q: x + 3 = 5; x = ?
        // A: 2
        case 1:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});
            int c = a + b;

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";

            std::string q = aStr + " + x = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                q = "x + " + bStr + " = " + std::to_string(c);
                std::swap(a, b);
            }
            q += "\nx = ?";

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                q,
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: x - 3 = 5; x = ?
        // A: 8
        case 2:  return [easy]() {
            diceFunction dice;
            if (easy) {
                dice = createDice(-10, 10);
            } else {
                dice = createDice(-20, 20);
            }

            int a = dice({0});
            int b = dice({0, a});
            int c = a - b;

            std::string aStr = std::to_string(a);
            std::string bStr = std::to_string(b);
            if (a < 0) aStr = "(" + aStr + ")";
            if (b < 0) bStr = "(" + bStr + ")";

            std::string q = aStr + " - x = " + std::to_string(c);
            if (createDice(0, 1)({}) == 0) {
                q = "x - " + bStr + " = " + std::to_string(c);
                std::swap(a, b);
            }
            q += "\nx = ?";

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                q,
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };


        // Q: 3x *ADD/SUB* 8 = 19; x = ?
        // A: 9
        case 3: return [easy]() {
            diceFunction diceA, diceB;
            if (easy) {
                diceA = createDice(2, 6);
                diceB = createDice(2, 15);
            } else {
                diceA = createDice(2, 10);
                diceB = createDice(5, 25);
            }

            int a = diceA({});
            int b = diceA({});
            int c = diceB({});

            int d = (a * b) - c;
            std::string op = "-";
            if (createDice(0, 1)({}) == 0) {
                d = (a * b) + c;
                op = "+";
            }

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                std::to_string(a) + "x " + op + " " + std::to_string(c) + " = " + std::to_string(d) + "\nx = ?",
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 4x *ADD/SUB* 6 = 6x *ADD/SUB* 16; x = ?
        // A: 5
        case 4: return [easy]() {
            diceFunction diceA, diceB;
            if (easy) {
                diceA = createDice(2, 6);
                diceB = createDice(2, 15);
            } else {
                diceA = createDice(2, 10);
                diceB = createDice(5, 25);
            }

            int a = diceA({});
            int b = diceA({});
            int c = diceB({});
            int d = (a * b) - c;
            std::string op = "-";
            if (createDice(0, 1)({}) == 0) {
                d = (a * b) + c;
                op = "+";
            }
            std::string left = std::to_string(a) + "x " + op + " " + std::to_string(c);

            auto diceDiff = createDice(-4, 4);
            int a2;
            do {
                a2 = a + diceDiff({0});
            } while (a2 == 0);

            int c2 = d - (a2 * b);
            std::string op2 = "+";
            if (c2 < 0) {
                c2 = std::abs(c2);
                op2 = "-";
            }
            std::string right = std::to_string(a2) + "x " + op2 + " " + std::to_string(c2);

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                left + " = " + right + "\nx = ?",
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: 7(x *ADD/SUB* 5) = 28; x = ?
        // A: 9
        case 5: return [easy]() {
            diceFunction diceA, diceB;
            if (easy) {
                diceA = createDice(2, 6);
                diceB = createDice(2, 15);
            } else {
                diceA = createDice(2, 10);
                diceB = createDice(5, 25);
            }

            int a = diceA({});
            int b = diceA({});
            int c = diceB({});

            int d = a * (b - c);
            std::string op = "-";
            if (createDice(0, 1)({}) == 0) {
                d = a * (b + c);
                op = "+";
            }

            auto wDice = createDiceRange(b);
            int w1 = wDice({b});
            int w2 = wDice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                std::to_string(a) + "(x " + op + " " + std::to_string(c) + ") = " + std::to_string(d) + "\nx = ?",
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: -24 / (x - 8) = 4; x = ?
        // A: 2
        case 6: return [easy]() {
            diceFunction diceA, diceB;
            if (easy) {
                diceA = createDice(2, 6);
                diceB = createDice(2, 15);
            } else {
                diceA = createDice(2, 10);
                diceB = createDice(5, 25);
            }

            int a, b, x, intC;
            float c;
            do {
                a = diceB({});
                b = diceA({});
                x = diceA({b});
                c = a / static_cast<float>(x - b);
                intC = static_cast<int>(c);
            } while (static_cast<float>(intC) != c);

            auto wDice = createDiceRange(x);
            int w1 = wDice({x});
            int w2 = wDice({x, w1});
            uniqueValues(x, w1, w2);

            return Question(
                std::to_string(a) + " / (x - " + std::to_string(b) + ") = " + std::to_string(intC) + "\nx = ?",
                std::to_string(x),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // Q: x *ADD/SUB* y = -2; y = 1; x = ?
        // A:
        case 7: return [easy] {
            diceFunction dice;
            if (easy) {
                dice = createDice(0, 18);
            } else {
                dice = createDice(12, 32);
            }

            int a = dice({0});
            int b = dice({0, a});
            int c = a + b;
            std::string op = "+";

            if (createDice(0, 1)({}) == 0) {
                std::swap(c, a);
                op = "-";
            }

            std::string aStr = std::to_string(a);
            std::string bStr = "?";
            if (createDice(0, 1)({}) == 0) {
                aStr = "?";
                bStr = std::to_string(b);
                std::swap(a, b);
            }

            int w1 = dice({b});
            int w2 = dice({b, w1});
            uniqueValues(b, w1, w2);

            return Question(
                "x " + op + " y = " + std::to_string(c) + "\nx = " + aStr + "\ny = " + bStr,
                std::to_string(b),
                std::to_string(w1),
                std::to_string(w2)
            );
        };

        // MIXED
        case 8: return [easy] {
            auto number = createDice(1, 7)({});
            auto gen = getGeneratorEquations(number, easy);
            return gen();
        };

        default: throw new std::range_error("invalid puzzel number id");
    }
}

cocos2d::extension::HSV getHsvColorStep()
{
    static cocos2d::extension::HSV out;
    out.h = 0.0;
    out.s = 0.0;
    out.v = -0.20;
    return out;
}

} // namespace config
