#ifndef MATH_FLURRY_H
#define MATH_FLURRY_H

#include <string>

class MyFlurry
{
public:
    static bool stopLogging;
    
    static void logEvent(const std::string& name);
    static void logEventWithType(const std::string& name, const std::string& type);
    static void startTimedEvent(const std::string& name);
    static void endTimedEvent(const std::string& name);
};

#endif // MATH_FLURRY_H