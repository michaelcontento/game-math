#ifndef MATH_GAMEPAGE_H
#define MATH_GAMEPAGE_H

#include <unordered_set>
#include "cocos2d.h"
#include "Page.h"

class AnswerButton;

class GamePage : public Page
{
public:
    static GamePage* create(void) = delete;
    static GamePage* create(const Page* const parentPage);
    bool init(const Page* const parentPage);
    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    const Page* parentPage = nullptr;
    std::unordered_set<AnswerButton*> answerButtons {};
    cocos2d::Node* question = nullptr;

    void addQuestion();
    void setNextQuestion();
    void addAnswerButtons();
    void configureAndAlignQuestionLabel(cocos2d::LabelTTF& label) const;
    cocos2d::Color3B getNextAnswerButtonColor(const cocos2d::Color3B& color) const;
};

#endif // MATH_GAMEPAGE_H