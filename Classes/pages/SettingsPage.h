#ifndef MATH_SETTINGSPAGE_H
#define MATH_SETTINGSPAGE_H

#include "Page.h"

class SettingsPage : public Page
{
public:
    CREATE_FUNC(SettingsPage);
    bool init() override;
};

#endif // MATH_SETTINGSPAGE_H