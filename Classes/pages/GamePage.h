#ifndef MATH_GAMEPAGE_H
#define MATH_GAMEPAGE_H

#include <vector>
#include <unordered_set>
#include "cocos2d.h"
#include "Page.h"
#include "../puzzle/Question.h"

class AnswerButton;

class GamePage : public Page
{
public:
    static GamePage* create(void) = delete;
    static GamePage* create(const Page& parentPage);
    bool init(const Page& parentPage);
    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

    void markQuestionAnswered();
    bool allQuestionsAnswered() const;

private:
    static constexpr unsigned short questionAmount = 10;
    const Page* parentPage = nullptr;
    cocos2d::Node* progressBar = nullptr;
    cocos2d::LabelTTF* question = nullptr;
    std::vector<AnswerButton*> answerButtons {};
    std::unordered_set<puzzle::Question> questions {};

    void addQuestion();
    void addAnswerButtons();
    void addProgressbar();

    void updateProgressbar();
    void setNextQuestion();
    void configureAndAlignQuestionLabel(cocos2d::LabelTTF& label) const;
    cocos2d::Color3B getNextAnswerButtonColor(const cocos2d::Color3B& color) const;
};

#endif // MATH_GAMEPAGE_H