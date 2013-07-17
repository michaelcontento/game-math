#include "GamePage.h"

#include "../utils/color.h"
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../buttons/AnswerButton.h"
#include "../buttons/HintButton.h"
#include "../puzzle/Generator.h"
#include "../GameTimer.h"
#include "PageManager.h"

using namespace cocos2d;

GamePage* GamePage::create(const Page& parentPage)
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

bool GamePage::init(const Page& parentPage)
{
    if (!Page::init()) {
        return false;
    }

    this->parentPage = &parentPage;
    setBackground(Color3B::WHITE);

    puzzle::Generator generator {
        "{number} {operator} {number}",
        "{number}",
        {puzzle::Operator::PLUS},
        {puzzle::NumberRange::SMALL}
    };
    while (questions.size() < questionAmount) {
        questions.insert(generator.generate());
    }

    addTimer();
    addHints();
    addQuestion();
    addAnswerButtons();
    addProgressbar();

    return true;
}

void GamePage::addTimer()
{
    timer = GameTimer::create(*this);
    addChild(timer);

    timer->setAnchorPoint({1, 1});

    timer->setPositionX(config::getFrameSize().width);
    timer->setPositionY(config::getFrameSize().height - config::getProgressbarHeight());
}

void GamePage::addHints()
{
    auto hints = HintButton::create(*this);
    addChild(hints);

    hints->setAnchorPoint({0.5, 1});

    hints->setPositionX(config::getFrameSize().width / 2);
    hints->setPositionY(config::getFrameSize().height - config::getProgressbarHeight());
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
       CallFunc::create([this]() { setNextQuestion(); timer->start(); }),
       NULL
    ));
}

void GamePage::addProgressbar()
{
    // == DrawNode
    auto height = config::getProgressbarHeight();
    Point verts[] = {
        {0, 0}, {0, height},
        {getContentSize().width, height}, {getContentSize().width, 0}
    };

    auto color = color::toRGBA(parentPage->getBackground());
    auto drawNode = DrawNode::create();
    drawNode->drawPolygon(verts, 4, color, 1, color);

    // == Node
    progressBar = Node::create();
    addChild(progressBar);

    progressBar->addChild(drawNode);
    progressBar->setContentSize({getContentSize().width, height});
    progressBar->setAnchorPoint({1, 1});
    progressBar->setPosition({0, getContentSize().height});

}

void GamePage::updateProgressbar()
{
    auto solved = questionAmount - questions.size();
    auto progress = solved / float(questionAmount);

    auto newPos = progressBar->getPosition();
    newPos.x = getContentSize().width * progress;

    progressBar->stopAllActions();
    progressBar->runAction(
        EaseInOut::create(MoveTo::create(0.25, newPos), 2)
    );
}

bool GamePage::allQuestionsAnswered() const
{
    return questions.empty();
}

void GamePage::handleAllQuestionsAnswered()
{
    if (allAnsweredAlreadyHandled) {
        return;
    }
    allAnsweredAlreadyHandled = true;

    PageManager::shared().scrollUp();
    CCLog("DONE");
}

void GamePage::timeover()
{
    if (!timeoverAlreadyHandled) {
        timeoverAlreadyHandled = true;
        handleTimeover();
    }
}

bool GamePage::isTimeover() const
{
    return timeoverAlreadyHandled;
}

bool GamePage::isStarted() const
{
    return timer->isStarted();
}

void GamePage::handleTimeover()
{
    CCLog("TIMEOVER");
}

void GamePage::markQuestionAnswered()
{
    if (!question) {
        throw new std::runtime_error("no question present -- call addQuestion() first");
    }

    questions.erase(questions.begin());
    updateProgressbar();

    if (allQuestionsAnswered()) {
        handleAllQuestionsAnswered();
        return;
    }

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
    auto& currentQuestion = *questions.begin();
    std::vector<std::string> answers = {
        currentQuestion.rightAnswer,
        currentQuestion.wrongAnswer1,
        currentQuestion.wrongAnswer2
    };

    std::random_shuffle(answers.begin(), answers.end());
    std::random_shuffle(answerButtons.begin(), answerButtons.end());

    for (const auto& btn : answerButtons) {
        auto answer = answers.back();
        answers.pop_back();

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

void GamePage::answeredWrong()
{
}

void GamePage::answeredRight()
{
    markQuestionAnswered();
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
        auto btn = AnswerButton::create(*this, color);
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

bool GamePage::isVisible() const
{
    return true;
}