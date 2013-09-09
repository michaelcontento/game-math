#ifndef MATH_ANSWERBUTTON_H
#define MATH_ANSWERBUTTON_H

#include "cocos2d.h"
#include "../utils/config.h"

class GamePage;

class AnswerButton : public cocos2d::Node, public cocos2d::TouchDelegate
{
public:
    bool enabled = true;
    
    static AnswerButton* create(void) = delete;
    static AnswerButton* create(GamePage& page, const cocos2d::Color3B& color);
    bool init(GamePage& page, const cocos2d::Color3B& color);

    void setIsRight(const bool flag);
    bool isRightAnswer() const;
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
    cocos2d::Node* container = nullptr;
    float lastContainerPos = 0;
    cocos2d::LabelTTF* indicatorLabelLeft = nullptr;
    cocos2d::LabelTTF* indicatorLabelRight = nullptr;
    const float startPosLeft  = (50 * config::getScaleFactor()) * -1;
    const float startPosRight = (50 * config::getScaleFactor()) + config::getFrameSize().width;
    const float endPosLeft    = (60 * config::getScaleFactor());
    const float endPosRight   = (60 * config::getScaleFactor()) * -1 + config::getFrameSize().width;
    bool isRight = false;
    bool hasBeenUsed = true;

    cocos2d::LabelTTF* addSubLabel(const std::string& text, const int fontSize);
    bool replaceSqrt(const std::string& text);
    bool replaceFraction(const std::string& text);
    void addBackground(const cocos2d::Color3B& color);
    void addLabels();
};

#endif // MATH_ANSWERBUTTON_H