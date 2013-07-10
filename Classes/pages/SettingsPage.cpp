#include "SettingsPage.h"

using namespace cocos2d;

bool SettingsPage::init()
{
    if (!Page::init()) {
        return false;
    }

    setBackground(Color3B::RED);

    return true;
}