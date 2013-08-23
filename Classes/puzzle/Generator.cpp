#include "Generator.h"

namespace puzzle {

std::random_device Generator::randDevice;

int Generator::dice(const std::unordered_set<int> ignore)
{
    std::uniform_int_distribution<> dice(0, 12);

    int result = dice(randDevice);
    while (ignore.count(result) > 0) {
        result = dice(randDevice);
    }
    
    return result;
}

Question Generator01::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);
    
    int w1 = dice({c});
    int w2 = dice({c, w1});

    return {
        std::to_string(a) + " " + std::to_string(op) + " " + std::to_string(b),
        std::to_string(c),
        std::to_string(w1),
        std::to_string(w2)
    };
}

Question Generator02::generate()
{
    auto op = *operators.begin();
    
    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);

    int w1 = dice({b});
    int w2 = dice({b, w1});

    return {
        std::to_string(a) + " " + std::to_string(op) + " ? = " + std::to_string(c),
        std::to_string(b),
        std::to_string(w1),
        std::to_string(w2)
    };
}

Question Generator03::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);

    int w1, w2, cw1 = 0;
    do {
        w1 = dice({a});
        w2 = dice({a, w1});
        cw1 = calculate(op, w1, w2);
    } while (cw1 == c);
    
    int w3, w4, cw2 = 0;
    do {
        w3 = dice({a, w1});
        w4 = dice({a, w1, w3});
        cw2 = calculate(op, w3, w4);
    } while (cw2 == c || cw2 == cw1);

    return {
        std::to_string(c),
        std::to_string(a)  + " " + std::to_string(op) + " " + std::to_string(b),
        std::to_string(w1) + " " + std::to_string(op) + " " + std::to_string(w2),
        std::to_string(w3) + " " + std::to_string(op) + " " + std::to_string(w4)
    };
}

Question Generator04::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);

    int w1, w2, cw1 = 0;
    do {
        w1 = dice({a});
        w2 = dice({a, w1});
        cw1 = calculate(op, w1, w2);
    } while (cw1 == c);

    int w3, w4, cw2 = 0;
    do {
        w3 = dice({a, w1,});
        w4 = dice({a, w1, w3});
        cw2 = calculate(op, w3, w4);
    } while (cw2 == c || cw2 == cw1);

    std::vector<int> sort {c, cw1, cw2};
    std::sort(sort.begin(), sort.end());

    int l1, l2, l3, l4, l5, l6 = 0;
    if (sort.front() == c) {
        l1 = a;
        l2 = b;
        l3 = w1;
        l4 = w2;
        l5 = w3;
        l6 = w4;
    } else if (sort.front() == cw1)  {
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

    return {
        "Smallest?",
        std::to_string(l1) + " " + std::to_string(op) + " " + std::to_string(l2),
        std::to_string(l3) + " " + std::to_string(op) + " " + std::to_string(l4),
        std::to_string(l5) + " " + std::to_string(op) + " " + std::to_string(l6)
    };
}

Question Generator05::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = dice({a, b});
    int d = calculate(op, calculate(op, a, b), c);

    int w1 = dice({c});
    int w2 = dice({c, w1});

    return {
        std::to_string(a) + " " + std::to_string(op) + " " + std::to_string(b) + " " + std::to_string(op) + " " + std::to_string(c),
        std::to_string(d),
        std::to_string(w1),
        std::to_string(w2)
    };

    // TODO: Wrong answers are to small!
}

Question Generator06::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);

    int c1, c2, tmp = 0;
    do {
        c1 = dice({a});
        c2 = dice({a, b});
        tmp = calculate(op, c1, c2);
    } while (tmp != c);

    int w1, w2, cw1 = 0;
    do {
        w1 = dice({a});
        w2 = dice({a, w1});
        cw1 = calculate(op, w1, w2);
    } while (cw1 == c);

    int w3, w4, cw2 = 0;
    do {
        w3 = dice({a, w1});
        w4 = dice({a, w1, w3});
        cw2 = calculate(op, w3, w4);
    } while (cw2 == c || cw2 == cw1);

    return {
        std::to_string(a)  + " " + std::to_string(op) + " " + std::to_string(b),
        std::to_string(c1) + " " + std::to_string(op) + " " + std::to_string(c2),
        std::to_string(w1) + " " + std::to_string(op) + " " + std::to_string(w2),
        std::to_string(w3) + " " + std::to_string(op) + " " + std::to_string(w4)
    };
}

Question Generator07::generate()
{
    auto op = *operators.begin();

    int a = dice();
    int b = dice({a});
    int c = calculate(op, a, b);

    int w1, w2, cw1 = 0;
    do {
        w1 = dice({a});
        w2 = dice({a, w1});
        cw1 = calculate(op, w1, w2);
    } while (cw1 == c);

    int w3, w4, cw2 = 0;
    do {
        w3 = dice({a, w1,});
        w4 = dice({a, w1, w3});
        cw2 = calculate(op, w3, w4);
    } while (cw2 == c || cw2 == cw1);

    std::vector<int> sort {c, cw1, cw2};
    std::sort(sort.begin(), sort.end());

    int l1, l2, l3, l4, l5, l6 = 0;
    if (sort.back() == c) {
        l1 = a;
        l2 = b;
        l3 = w1;
        l4 = w2;
        l5 = w3;
        l6 = w4;
    } else if (sort.back() == cw1)  {
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

    return {
        "Largest?",
        std::to_string(l1) + " " + std::to_string(op) + " " + std::to_string(l2),
        std::to_string(l3) + " " + std::to_string(op) + " " + std::to_string(l4),
        std::to_string(l5) + " " + std::to_string(op) + " " + std::to_string(l6)
    };
}

Question Generator08::generate()
{
    std::uniform_int_distribution<> dice(1, 7);

    switch (dice(randDevice)) {
        case 1: return std::make_shared<Generator01>(operators, ranges)->generate();
        case 2: return std::make_shared<Generator02>(operators, ranges)->generate();
        case 3: return std::make_shared<Generator03>(operators, ranges)->generate();
        case 4: return std::make_shared<Generator04>(operators, ranges)->generate();
        case 5: return std::make_shared<Generator05>(operators, ranges)->generate();
        case 6: return std::make_shared<Generator06>(operators, ranges)->generate();
        case 7: return std::make_shared<Generator07>(operators, ranges)->generate();
    }

    throw std::runtime_error("invalid generator id generated");
}

} // namespace puzzle