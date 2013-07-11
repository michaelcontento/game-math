#include "GamePage.h"

#include <deque>
#include "../utils/color.h"
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../buttons/AnswerButton.h"
#include "../puzzle/Generator.h"
#include "PageManager.h"

using namespace cocos2d;

GamePage* GamePage::create(const Page* const parentPage)
{
    GamePage* pRet = new GamePage();
    if (pRet && pRet->init(parentPage)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GamePage::init(const Page* const parentPage)
{
    if (!Page::init()) {
        return false;
    }

    this->parentPage = parentPage;
    setBackground(Color3B::WHITE);

    puzzle::Generator generator {
        "{number} {operator} {number}",
        "{number}",
        {puzzle::Operator::PLUS},
        {puzzle::NumberRange::SMALL}
    };
    for (int i = 0; i < questionAmount; ++i) {
        questions.push_back(generator.generate());
    }

    addQuestion();
    addAnswerButtons();

    return true;
}

void GamePage::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (allQuestionsAnswered()) {
        PageManager::shared().scrollUp();
    } else {
        markQuestionAnswered();
    }
}

void GamePage::addQuestion()
{
    if (question) {
        throw new std::runtime_error("question already present");
    }

    question = fonts::createNormal("ready?", 96);
    addChild(question);
    configureAndAlignQuestionLabel(*question);

    question->runAction(Sequence::create(
       EaseIn::create(FadeOut::create(config::getQuestionStartDelay()), 3),
       CallFunc::create([this]() { question->setString("3"); }),
       FadeOut::create(config::getQuestionFadeTime()),
       CallFunc::create([this]() { question->setString("2"); }),
       FadeOut::create(config::getQuestionFadeTime()),
       CallFunc::create([this]() { question->setString("1"); }),
       CallFunc::create([this]() { markQuestionAnswered(); }),
       NULL
    ));
}

bool GamePage::allQuestionsAnswered() const
{
    return (questions.size() <= 1);
}

void GamePage::markQuestionAnswered()
{
    if (!question) {
        throw new std::runtime_error("no question present -- call addQuestion() first");
    }

    if (allQuestionsAnswered()) {
        CCLog("DONE!");
        return;
    }

    questions.pop_front();
    question->runAction(Sequence::create(
        FadeOut::create(config::getQuestionFadeTime()),
        CallFunc::create([this]() { setNextQuestion(); }),
        NULL
    ));

    for (const auto& btn : answerButtons) {
        btn->fadeOutAnswer(config::getQuestionFadeTime());
    }
}

void GamePage::setNextQuestion()
{
    auto currentQuestion = *questions.begin();
    std::deque<std::string> answers = {
        currentQuestion.rightAnswer,
        currentQuestion.wrongAnswer1,
        currentQuestion.wrongAnswer2
    };

    std::random_shuffle(answers.begin(), answers.end());
    std::random_shuffle(answerButtons.begin(), answerButtons.end());

    for (const auto& btn : answerButtons) {
        auto answer = *answers.begin();
        answers.pop_front();

        btn->setIsRight(answer == currentQuestion.rightAnswer);
        btn->setAnswerString(answer);
        btn->showAnswer();
    }

    question->setString(currentQuestion.question.c_str());
    question->setOpacity(255);
}

void GamePage::configureAndAlignQuestionLabel(cocos2d::LabelTTF& label) const
{
    // color
    label.setColor(Color3B::BLACK);
 
    // alignment
    label.setAnchorPoint({0.5, 0.5});
    label.setHorizontalAlignment(kTextAlignmentCenter);
    label.setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    label.setPosition({
        getContentSize().width / 2,
        (getContentSize().height / 3 * 2) - (25 * config::getScaleFactor())
    });
}

void GamePage::addAnswerButtons()
{
    auto container = Node::create();
    addChild(container);

    auto containerSize = container->getContentSize();
    auto color = parentPage->getBackground();
    auto spacing = 30 * config::getScaleFactor();
    constexpr int answers = 3;

    for (int i = 1; i <= answers; ++i) {
        auto btn = AnswerButton::create(color);
        btn->hideAnswer();

        container->addChild(btn);
        answerButtons.push_back(btn);

        auto posY = (btn->getContentSize().height + spacing) * (answers - i);
        btn->setPosition({0, posY});
        
        containerSize.width  = std::max(containerSize.width,  btn->getPositionX() + btn->getContentSize().width);
        containerSize.height = std::max(containerSize.height, btn->getPositionY() + btn->getContentSize().height);

        color = getNextAnswerButtonColor(color);
    }

    container->setContentSize(containerSize);
    container->setAnchorPoint({0, 0});
    container->setPosition({0, spacing});
}

cocos2d::Color3B GamePage::getNextAnswerButtonColor(const cocos2d::Color3B& color) const
{
    auto hsvColor = color::toHSV(color);

    auto step = config::getHsvColorStep();
    hsvColor.h += step.h;
    hsvColor.s += step.s;
    hsvColor.v += step.v;

    return color::toRGB(hsvColor);
}