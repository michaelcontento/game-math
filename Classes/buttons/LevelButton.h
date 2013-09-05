#ifndef MATH_LEVELBUTTON_H
#define MATH_LEVELBUTTON_H

#include "cocos2d.h"

class CategoryPage;

class LevelButton : public cocos2d::Node
{
public:
    static LevelButton* create(void) = delete;
    static LevelButton* create(const int group, const int level, CategoryPage& parentPage);
    bool init(const int group, const int level, CategoryPage& parentPage);

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event);
    void changeOpacity(const int group, const int level);
    bool isLocked() const;

private:
    int group = -1;
    int level = -1;
    bool unlocked = false;
    CategoryPage* parentPage = nullptr;
    cocos2d::DrawNode* draw = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    cocos2d::Node* starContainer = nullptr;
    std::deque<cocos2d::Sprite*> stars {};

    void configureSize();
    void addBackground();
    cocos2d::Color4F getBackgroundColorFromParentPage() const;
    void addNumber();
    bool hasBeenTouched(cocos2d::Touch& touch, cocos2d::Event& event);
    void initializeStarContainerIfRequired();
    void refillStarContainerIfRequired(const int amount);
    void updateOpacity();
    bool ignoreNotification(const int group, const int level);
};

#endif // MATH_LEVELBUTTON_H