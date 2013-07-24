#ifndef MATH_LOCKEDCATEGORYPAGE_H
#define MATH_LOCKEDCATEGORYPAGE_H

#include "cocos2d.h"
#include "Page.h"

class LevelButton;

class LockedCategoryPage : public Page
{
public:
    static LockedCategoryPage* create(void) = delete;
    static LockedCategoryPage* create(const int group);
    bool init(const int group);
    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

private:
    int group = -1;
    cocos2d::DrawNode* draw = nullptr;

    void addHeadlineLabel();
    const std::string getHeadline() const;
    void addSublineLabel();
    void addCategoryBoxes();
    void addDescriptionLabel();
    void addPlayButton();
};

#endif // MATH_LOCKEDCATEGORYPAGE_H