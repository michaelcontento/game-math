#ifndef MATH_LEVELBUTTON_H
#define MATH_LEVELBUTTON_H

#include "cocos2d.h"

class Page;

class LevelButton : public cocos2d::Node
{
public:
    static LevelButton* create(void) = delete;
    static LevelButton* create(const int group, const int level, const Page& parentPage);
    bool init(const int group, const int level, const Page& parentPage);

    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event);
    void changeOpacity(const int group, const int level);

private:
    int group = -1;
    int level = -1;
    bool unlocked = false;
    const Page* parentPage = nullptr;
    cocos2d::DrawNode* draw = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    cocos2d::Node* starContainer = nullptr;
    std::deque<cocos2d::Sprite*> stars {};

    void configureSize();
    void addBackground();
    const cocos2d::Color4F getBackgroundColorFromParentPage() const;
    void addNumber();
    bool hasBeenTouched(cocos2d::Touch& touch, cocos2d::Event& event);
    void initializeStarContainerIfRequired();
    void refillStarContainerIfRequired(const int amount);
    void updateOpacity();
    bool ignoreNotification(const int group, const int level);
};

#endif // MATH_LEVELBUTTON_H