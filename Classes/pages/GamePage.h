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

class GamePage : public Page
{
public:
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

private:
    static constexpr unsigned short questionAmount = 10;
    const Page* parentPage = nullptr;
    GameTimer* timer = nullptr;
    HintButton* hints = nullptr;
    cocos2d::Node* progressBarLeft = nullptr;
    cocos2d::Node* progressBarRight = nullptr;
    cocos2d::LabelTTF* question = nullptr;
    std::vector<AnswerButton*> answerButtons {};
    std::unordered_set<Question> questions {};
    bool timeoverAlreadyHandled = false;
    cocos2d::Node* starContainer = nullptr;
    const float spacing = 25 * config::getScaleFactor();
    std::deque<cocos2d::Sprite*> stars {};
    static constexpr int maxStars = 3;
    bool acceptAnswers = false;
    int group = -1;
    int level = -1;

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
    void configureAndAlignQuestionLabel(cocos2d::LabelTTF& label) const;
    cocos2d::Color3B getNextAnswerButtonColor(const cocos2d::Color3B& color) const;
};

#endif // MATH_GAMEPAGE_H