#ifndef MATH_CATEGORYPAGE_H
#define MATH_CATEGORYPAGE_H

#include "Page.h"

class CategoryPage : public Page
{
public:
    CREATE_FUNC(CategoryPage);
    bool init() override;
    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    void addHeadlineLabel();
};

#endif // MATH_CATEGORYPAGE_H