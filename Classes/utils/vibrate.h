#ifndef MATH_VIBRATE_H
#define MATH_VIBRATE_H

#include <avalon/utils/platform.h>

#if AVALON_PLATFORM_IS_IOS
    #import <AudioToolbox/AudioServices.h>
#elif AVALON_PLATFORM_IS_TIZEN
    #include <FSystem.h>
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
#elif AVALON_PLATFORM_IS_TIZEN
    static Tizen::System::Vibrator vibrator;
    static bool constructed = false;

    if (!constructed) {
        vibrator.Construct();
        constructed = true;
    }

    static Tizen::System::IntensityDurationVibrationPattern pattern[1] = {{500, -1}};
    static int length = sizeof(pattern);
    vibrator.Start(pattern, length);
#endif
}

} // namespace vibrate

#endif /* MATH_VIBRATE_H */
