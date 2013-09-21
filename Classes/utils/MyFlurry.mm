#include "MyFlurry.h"

#include "cocos2d.h"
#import "Flurry.h"

bool MyFlurry::stopLogging = false;

void MyFlurry::logEvent(const std::string& name)
{
    if (stopLogging) return;
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()]];
}

void MyFlurry::logEventWithType(const std::string& name, const std::string& type)
{
    if (stopLogging) return;
    NSDictionary *articleParams = [NSDictionary dictionaryWithObjectsAndKeys:
        [NSString stringWithUTF8String:type.c_str()],
        @"type",
        nil
    ];
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()] withParameters:articleParams];
}

void MyFlurry::startTimedEvent(const std::string& name)
{
    if (stopLogging) return;
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()] timed:YES];
}

void MyFlurry::endTimedEvent(const std::string& name)
{
    if (stopLogging) return;
    [Flurry endTimedEvent:[NSString stringWithUTF8String:name.c_str()] withParameters:nil];
}