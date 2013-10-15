#ifndef MATH_ANSWERBUTTON_H
#define MATH_ANSWERBUTTON_H

#include "cocos2d.h"
#include "../utils/config.h"

class GamePage;
class QuestionString;

class AnswerButton : public cocos2d::Layer
{
public:
    AnswerButton()
    : enabled(true)
    , page(nullptr)
    , indicatorLabelLeft(nullptr)
    , indicatorLabelRight(nullptr)
    , startPosLeft(0)
    , startPosRight(0)
    , endPosLeft(0)
    , endPosRight(0)
    , question(nullptr)
    , isRight(false)
    , hasBeenUsed(true)
    {
        startPosLeft = (50 * config::getScaleFactor()) * -1;
        startPosRight = (50 * config::getScaleFactor()) + config::getFrameSize().width;
        endPosLeft = (60 * config::getScaleFactor());
        endPosRight = (60 * config::getScaleFactor()) * -1 + config::getFrameSize().width;
    };

    bool enabled;

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
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    GamePage* page;
    cocos2d::LabelTTF* indicatorLabelLeft;
    cocos2d::LabelTTF* indicatorLabelRight;
    float startPosLeft;
    float startPosRight;
    float endPosLeft;
    float endPosRight;
    QuestionString* question;
    bool isRight;
    bool hasBeenUsed;

    void addBackground(const cocos2d::Color3B& color);
    void addLabels();
};

#endif // MATH_ANSWERBUTTON_H
