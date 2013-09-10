#include "MyFlurry.h"

#include <avalon/utils/platform.h>
#include "cocos2d.h"
#import "Flurry.h"

bool MyFlurry::stopLogging = false;

void MyFlurry::logEvent(const std::string& name)
{
    if (stopLogging) return;
    cocos2d::log("[MyFlurry::logEvent] %s", name.c_str());
#if AVALON_PLATFORM_IS_IOS
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()]];
#endif
}

void MyFlurry::logEventWithType(const std::string& name, const std::string& type)
{
    if (stopLogging) return;
    cocos2d::log("[MyFlurry::logEventWithType] %s %s", name.c_str(), type.c_str());
#if AVALON_PLATFORM_IS_IOS
    NSDictionary *articleParams = [NSDictionary dictionaryWithObjectsAndKeys:
        @"type",
        [NSString stringWithUTF8String:type.c_str()],
        nil
    ];
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()] withParameters:articleParams];
#endif
}

void MyFlurry::startTimedEvent(const std::string& name)
{
    if (stopLogging) return;
    cocos2d::log("[MyFlurry::startTimedEvent] %s", name.c_str());
#if AVALON_PLATFORM_IS_IOS
    [Flurry logEvent:[NSString stringWithUTF8String:name.c_str()] timed:YES];
#endif
}

void MyFlurry::endTimedEvent(const std::string& name)
{
    if (stopLogging) return;
    cocos2d::log("[MyFlurry::endTimedEvent] %s", name.c_str());
#if AVALON_PLATFORM_IS_IOS
    [Flurry endTimedEvent:[NSString stringWithUTF8String:name.c_str()] withParameters:nil];
#endif
}