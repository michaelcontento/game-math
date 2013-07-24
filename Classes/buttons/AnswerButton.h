#ifndef MATH_ANSWERBUTTON_H
#define MATH_ANSWERBUTTON_H

#include "cocos2d.h"

class GamePage;

class AnswerButton : public cocos2d::Node, public cocos2d::TouchDelegate
{
public:
    static AnswerButton* create(void) = delete;
    static AnswerButton* create(GamePage& page, const cocos2d::Color3B& color);
    bool init(GamePage& page, const cocos2d::Color3B& color);

    void setIsRight(const bool flag);
    void setAnswerString(const std::string& text);

    void fadeOutAnswer(const float duration);
    void hideAnswer();
    void showAnswer();
    bool isAnswerVisible() const;

    void onEnter() override;
    void onExit() override;
    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    GamePage* page = nullptr;
    cocos2d::LabelTTF* label = nullptr;
    bool isRight = false;
    
    void addBackground(const cocos2d::Color3B& color);
    void addLabel();
};

#endif // MATH_ANSWERBUTTON_H