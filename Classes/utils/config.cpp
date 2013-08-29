#include "config.h"

#include <random>
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
        log("[%d %d] %d", min, max, result);
        while (rolls < 5 && ignore.count(result) > 0) {
            result = dice();
            ++rolls;
            log("[%d %d] %d !", min, max, result);
        }
        return result;
    };
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
                "Smallest?",
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
            int q1 = qDice({b});
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
                "Largest?",
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
                "Smallest?",
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
                "Largest?",
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

            int range = std::max(3, std::min(c / 2, 17));
            auto qDice = createDice(std::max(0, c - range), c + range);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});

            return Question(
                std::to_string(a) + " x " + std::to_string(b),
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
                std::to_string(a)  + " x " + std::to_string(b),
                std::to_string(w1) + " x " + std::to_string(w2),
                std::to_string(w3) + " x " + std::to_string(w4)
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
                "Smallest?",
                std::to_string(l1) + " x " + std::to_string(l2),
                std::to_string(l3) + " x " + std::to_string(l4),
                std::to_string(l5) + " x " + std::to_string(l6)
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

            int range = std::max(3, std::min(d / 2, 27));
            auto qDice = createDice(d - range, d + range);
            int w1 = qDice({d});
            int w2 = qDice({d, w1});

            return Question(
                std::to_string(a) + " x " + std::to_string(b) + " x " + std::to_string(c),
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
                std::to_string(q1) + " x " + std::to_string(q2),
                std::to_string(a)  + " x " + std::to_string(b),
                std::to_string(w1) + " x " + std::to_string(w2),
                std::to_string(w3) + " x " + std::to_string(w4)
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
                "Largest?",
                std::to_string(l1) + " x " + std::to_string(l2),
                std::to_string(l3) + " x " + std::to_string(l4),
                std::to_string(l5) + " x " + std::to_string(l6)
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

            int range = std::max(3, std::min(c / 2, 17));
            auto qDice = createDice(std::max(0, c - range), c + range);
            int w1 = qDice({c});
            int w2 = qDice({c, w1});

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

            int range = std::max(3, std::min(a / 2, 17));
            auto qDice = createDice(std::max(0, a - range), a + range);
            int w1 = qDice({a});
            int w2 = qDice({a, w1});

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
                "Smallest?",
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

            int range = std::max(3, std::min(d / 2, 27));
            auto qDice = createDice(std::max(0, d - range), d + range);
            int w1 = qDice({d});
            int w2 = qDice({d, w1});

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
                "Largest?",
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

} // namespace config
