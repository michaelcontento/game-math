#ifndef  MATH_APPDELEGATE_H
#define  MATH_APPDELEGATE_H

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};

#endif // MATH_APPDELEGATE_H