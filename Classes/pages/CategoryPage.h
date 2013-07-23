#ifndef MATH_CATEGORYPAGE_H
#define MATH_CATEGORYPAGE_H

#include <vector>
#include "cocos2d.h"
#include "Page.h"

class LevelButton;

class CategoryPage : public Page
{
public:
    static CategoryPage* create(void) = delete;
    static CategoryPage* create(const int group);
    bool init(const int group);
    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

private:
    std::vector<LevelButton*> levelButtons {};
    int group = -1;
    
    void addHeadlineLabel();
    void addLevelButtons();
};

#endif // MATH_CATEGORYPAGE_H