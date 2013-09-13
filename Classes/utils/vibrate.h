#ifndef MATH_VIBRATE_H
#define MATH_VIBRATE_H

#import <AudioToolbox/AudioServices.h>

namespace vibrate {

void withBeepFallback()
{
    AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
}

void silenced()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

} // namespace vibrate

#endif /* MATH_VIBRATE_H */
