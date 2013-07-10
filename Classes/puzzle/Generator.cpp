#include "Generator.h"

namespace puzzle {

const Question Generator::generate() const
{
    return Question("1 + 1", "2", "5", "0");
};

} // namespace puzzle