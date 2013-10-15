#ifndef MATH_GAMEPAGE_H
#define MATH_GAMEPAGE_H

#include <vector>
#include <unordered_set>
#include "cocos2d.h"
#include "Page.h"
#include "../utils/config.h"
#include "../utils/Question.h"

class AnswerButton;
class GameTimer;
class HintButton;
class QuestionString;

class GamePage : public Page
{
public:
    GamePage()
    : revealable(false)
    , parentPage(nullptr)
    , timer(nullptr)
    , hints(nullptr)
    , progressBarLeft(nullptr)
    , progressBarRight(nullptr)
    , question(nullptr)
    , timeoverAlreadyHandled(false)
    , starContainer(nullptr)
    , spacing(0)
    , acceptAnswers(false)
    , group(-1)
    , level(-1)
    {
        spacing = 25 * config::getScaleFactor();
    };

    static GamePage* create(void) = delete;
    static GamePage* create(const int group, const int level, const Page& parentPage);
    bool init(const int group, const int level, const Page& parentPage);

    bool allQuestionsAnswered() const;
    void answeredWrong();
    void answeredRight();
    bool revealHint();

    void timeover();
    bool isTimeover() const;
    bool isStarted() const;

    bool isVisible() const override;
    bool canBeRevealed() const;

    void pause();
    void resume(const float delay = 0);

    virtual void onBackspace() override;

private:
    bool revealable;
    static constexpr unsigned short questionAmount = 10;
    const Page* parentPage;
    GameTimer* timer;
    HintButton* hints;
    cocos2d::Node* progressBarLeft;
    cocos2d::Node* progressBarRight;
    QuestionString* question;
    std::vector<AnswerButton*> answerButtons;
    std::unordered_set<Question> questions;
    bool timeoverAlreadyHandled;
    cocos2d::Node* starContainer;
    float spacing;
    std::deque<cocos2d::Sprite*> stars;
    static constexpr int maxStars = 3;
    bool acceptAnswers;
    int group;
    int level;

    void restart();
    void generateQuestions();
    void addTimer();
    void addStars();
    void addHints();
    void addBackButton();
    void addQuestion();
    void addAnswerButtons();
    void addProgressbar();

    cocos2d::Sprite* createStar() const;
    void handleAllQuestionsAnswered();
    void handleTimeover();
    void handleNoMoreStars();

    void markQuestionAnswered();
    void updateProgressbar();
    void setNextQuestion();
    cocos2d::Color3B getNextAnswerButtonColor(const cocos2d::Color3B& color) const;
};

#endif // MATH_GAMEPAGE_H
