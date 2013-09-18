#ifndef MATH_VIBRATE_H
#define MATH_VIBRATE_H

#include <avalon/utils/platform.h>

#if AVALON_PLATFORM_IS_IOS
    #import <AudioToolbox/AudioServices.h>
#endif

namespace vibrate {

void withBeepFallback()
{
#if AVALON_PLATFORM_IS_IOS
    AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
#endif
}

void silenced()
{
#if AVALON_PLATFORM_IS_IOS
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
#endif
}

} // namespace vibrate

#endif /* MATH_VIBRATE_H */
