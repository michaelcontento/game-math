#include "GamePage.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
using avalon::i18n::_;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <boost/foreach.hpp>
#include <avalon/Appirater.h>
#include <avalon/platform/android/gnustl_string_fixes.h>
#include "../utils/color.h"
#include "../utils/fonts.h"
#include "../utils/user.h"
#include "../utils/vibrate.h"
#include "../utils/MyFlurry.h"
#include "../utils/QuestionString.h"
#include "../buttons/AnswerButton.h"
#include "../buttons/HintButton.h"
#include "../buttons/BackButton.h"
#include "../GameTimer.h"
#include "../Alert.h"
#include "../PageManager.h"

using namespace cocos2d;

GamePage* GamePage::create(const int group, const int level, const Page& parentPage)
{
    GamePage* pRet = new GamePage();
    if (pRet && pRet->init(group, level, parentPage)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GamePage::init(const int group, const int level, const Page& parentPage)
{
    if (!Page::init()) {
        return false;
    }

    this->group = group;
    this->level = level;
    this->parentPage = &parentPage;
    setBackground(Color3B::WHITE);

    generateQuestions();
    addStars();
    addHints();
    addTimer();
    addBackButton();
    addQuestion();
    addAnswerButtons();
    addProgressbar();

    return true;
}

void GamePage::restart()
{
    questions.clear();
    generateQuestions();

    BOOST_FOREACH (auto& btn, answerButtons) {
        btn->enabled = true;
        btn->fadeOutAnswer(config::getQuestionFadeTime());
    }

    stars.clear();
    starContainer->removeAllChildren();
    fonts::fillStarContainer(*starContainer, stars, maxStars, Color3B::BLACK, false);

    updateProgressbar();

    if (timer) {
        timer->stop();
        timer->removeFromParent();
        timer = nullptr;
    }
    addTimer();

    timeoverAlreadyHandled = false;
    revealable = true;
    resume();

    if (question) {
        question->stopAllActions();
        question->runAction(Sequence::create(FadeOut::create(0.2), RemoveSelf::create(), NULL));
        question = nullptr;
    }
    addQuestion();
}

void GamePage::generateQuestions()
{
    auto generator = config::getGenerator(group, level);
    while (questions.size() < questionAmount) {
        questions.insert(generator());
    }
}

void GamePage::addTimer()
{
    timer = GameTimer::create(*this);
    addChild(timer);
#if !AVALON_PLATFORM_IS_TIZEN
    timer->setHintButton(*hints);
#endif

    const auto fix = 5 * config::getScaleFactor();

    timer->setAnchorPoint({1, 1});
    timer->setPositionX(config::getFrameSize().width - spacing);
    timer->setPositionY(starContainer->getPositionY() - starContainer->getContentSize().height + fix);
}

void GamePage::addBackButton()
{
    auto btn = BackButton::create();
    addChild(btn);

    btn->setAnchorPoint({0, 1});
    btn->setPositionX(spacing);
    btn->setPositionY(config::getFrameSize().height - spacing - btn->getContentSize().height);
}

void GamePage::addStars()
{
    starContainer = Node::create();
    addChild(starContainer);

    fonts::fillStarContainer(*starContainer, stars, maxStars, Color3B::BLACK, false);

    const auto fix = 10 * config::getScaleFactor();

    starContainer->setAnchorPoint({1, 1});
    starContainer->setPositionX(config::getFrameSize().width - spacing);
    starContainer->setPositionY(config::getFrameSize().height - spacing + fix);
}

void GamePage::addHints()
{
#if !AVALON_PLATFORM_IS_TIZEN
    hints = HintButton::create(*this);
    addChild(hints);

    const auto fix = 5 * config::getScaleFactor();

    hints->setAnchorPoint({0.5, 0.5});
    hints->setPositionX(config::getFrameSize().width / 2);
    hints->setPositionY(config::getFrameSize().height - spacing + fix - (hints->getContentSize().height / 2));
#endif
}

void GamePage::addQuestion()
{
    if (question) {
        throw new std::runtime_error("question already present");
    }

    question = QuestionString::create(114);
    question->setColor(Color3B::BLACK);
    question->setAnchorPoint({0.5, 0.5});
    question->setPosition({
        getContentSize().width / 2,
        (getContentSize().height / 3 * 2) - (25 * config::getScaleFactor())
    });
    question->setText("3");
    addChild(question);

    question->runAction(Sequence::create(
        EaseIn::create(FadeOut::create(config::getQuestionStartDelay() * 2), 3),
        CallFunc::create([this]() { question->setText("2"); }),
        FadeOut::create(config::getQuestionStartDelay()),
        CallFunc::create([this]() { question->setText("1"); }),
        FadeOut::create(config::getQuestionStartDelay()),
        CallFunc::create([this]() { setNextQuestion(); timer->start(); acceptAnswers = true; }),
        NULL
    ));
}

void GamePage::addProgressbar()
{
    // == DrawNode
    auto height = config::getProgressbarHeight();
    cocos2d::Point verts[] = {
        {0, 0}, {0, height},
        {getContentSize().width / 2, height}, {getContentSize().width / 2, 0}
    };
    auto color = color::toRGBA(parentPage->getBackground());

    auto drawNodeLeft = DrawNode::create();
    drawNodeLeft->drawPolygon(verts, 4, color, 1, color);

    auto drawNodeRight = DrawNode::create();
    drawNodeRight->drawPolygon(verts, 4, color, 1, color);

    // == Left
    progressBarLeft = Node::create();
    addChild(progressBarLeft);

    progressBarLeft->addChild(drawNodeLeft);
    progressBarLeft->setContentSize({getContentSize().width / 2, height});
    progressBarLeft->setAnchorPoint({1, 1});
    progressBarLeft->setPosition({0, getContentSize().height});

    // == Right
    progressBarRight = Node::create();
    addChild(progressBarRight);

    progressBarRight->addChild(drawNodeRight);
    progressBarRight->setContentSize({getContentSize().width, height});
    progressBarRight->setAnchorPoint({0, 1});
    progressBarRight->setPosition({getContentSize().width, getContentSize().height});
}

void GamePage::updateProgressbar()
{
    auto solved = questionAmount - questions.size();
    auto progress = solved / float(questionAmount);

    // Position

    auto newPosLeft = progressBarLeft->getPosition();
    newPosLeft.x = getContentSize().width * 0.5 * progress;

    auto newPosRight = progressBarLeft->getPosition();
    newPosRight.x = getContentSize().width - newPosLeft.x;

    // Action

    progressBarLeft->stopAllActions();
    progressBarLeft->runAction(EaseInOut::create(MoveTo::create(0.5, newPosLeft), 2));

    progressBarRight->stopAllActions();
    progressBarRight->runAction(EaseInOut::create(MoveTo::create(0.5, newPosRight), 2));
}

bool GamePage::allQuestionsAnswered() const
{
    return questions.empty();
}

void GamePage::handleAllQuestionsAnswered()
{
    BOOST_FOREACH (auto& btn, answerButtons) {
        btn->enabled = false;
    }

    acceptAnswers = false;
    timer->stop();

    user::setLevelStars(group, level, stars.size());
    avalon::Appirater::getInstance()->userDidSignificantEvent();

    auto alert = Alert::create();
    addChild(alert);
    alert->setDescription(_("game", "done").get());
    alert->addButton(_("game", "menu").get(), []() { PageManager::shared().scrollUp(); });
    if (level < 16) {
        alert->addButton(_("game", "next").get(), [this]() { ++level; restart(); });
    }
    alert->show([]() { });

    SimpleAudioEngine::getInstance()->playEffect("solved.mp3");
    MyFlurry::logEventWithType("solved", std::to_string(group) + "." + std::to_string(level));
}

void GamePage::handleNoMoreStars()
{
    BOOST_FOREACH (auto& btn, answerButtons) {
        btn->enabled = false;
    }

    acceptAnswers = false;

    auto alert = Alert::create();
    addChild(alert);
    alert->setDescription(_("game", "nomorestars").get());
    alert->addButton(_("game", "menu").get(), []() { PageManager::shared().scrollUp(); });
    alert->addButton(_("game", "restart").get(), [this]() { restart(); });
    alert->show([]() {});

    SimpleAudioEngine::getInstance()->playEffect("lost.mp3");
    MyFlurry::logEventWithType("nomorestars", std::to_string(group) + "." + std::to_string(level));
}

void GamePage::handleTimeover()
{
    BOOST_FOREACH (auto& btn, answerButtons) {
        btn->enabled = false;
    }

    acceptAnswers = false;
    timer->stop();

    auto alert = Alert::create();
    addChild(alert);
    alert->setDescription(_("game", "timeout").get());
    alert->addButton(_("game", "menu").get(), []() { PageManager::shared().scrollUp(); });
    alert->addButton(_("game", "restart").get(), [this]() { restart(); });
    alert->show([]() { });

    SimpleAudioEngine::getInstance()->playEffect("lost.mp3");
    MyFlurry::logEventWithType("timeout", std::to_string(group) + "." + std::to_string(level));
}

void GamePage::timeover()
{
    if (!timeoverAlreadyHandled) {
        timeoverAlreadyHandled = true;
        handleTimeover();
    }
}

void GamePage::pause()
{
    timer->pause();
}

void GamePage::resume(const float delay)
{
    timer->resume(delay);
}

bool GamePage::revealHint()
{
    if (!acceptAnswers) {
        return false;
    }

    BOOST_FOREACH (auto& btn, answerButtons) {
        if (!btn->isRightAnswer()) {
            btn->fadeOutAnswer(config::getQuestionFadeTime());
        }
    }
    revealable = false;

    MyFlurry::logEventWithType("hintused", std::to_string(group) + "." + std::to_string(level));
    return true;
}

bool GamePage::canBeRevealed() const
{
    return revealable;
}

bool GamePage::isTimeover() const
{
    return timeoverAlreadyHandled;
}

bool GamePage::isStarted() const
{
    return timer->isStarted();
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
    SimpleAudioEngine::getInstance()->playEffect("click.mp3");

    question->runAction(Sequence::create(
        FadeOut::create(config::getQuestionFadeTime()),
        CallFunc::create([this]() { setNextQuestion(); }),
        NULL
    ));

    BOOST_FOREACH (auto& btn, answerButtons) {
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

    BOOST_FOREACH (auto& btn, answerButtons) {
        auto answer = answers.back();
        answers.pop_back();

        btn->setIsRight(answer == currentQuestion.rightAnswer);
        btn->setAnswerString(answer);
        btn->showAnswer();
    }

    question->setText(currentQuestion.question.c_str());
    question->setOpacity(255);

    timer->resetHintTimer();
    revealable = true;
}

void GamePage::answeredWrong()
{
    if (!acceptAnswers) {
        return;
    }

    auto star = stars.front();
    stars.pop_front();
    star->setColor(Color3B::RED);
    star->runAction(Sequence::create(
        Spawn::create(
            FadeOut::create(0.5),
            ScaleTo::create(0.5, 1.25),
            NULL
        ),
        RemoveSelf::create(),
        NULL
    ));

    if (user::useVibration()) {
        vibrate::silenced();
    }

    if (stars.empty()) {
        timer->stop();
        timer->removeFromParent();
        timer = nullptr;
        handleNoMoreStars();
    } else {
        SimpleAudioEngine::getInstance()->playEffect("wrong.mp3");
    }
}

void GamePage::answeredRight()
{
    if (!acceptAnswers) {
        return;
    }

    markQuestionAnswered();
}

void GamePage::addAnswerButtons()
{
    auto container = Node::create();
    addChild(container);

    auto containerSize = container->getContentSize();
    auto color = parentPage->getBackground();
    auto spacing = config::getAnswerButtonPadding();
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
    hsvColor.h = fmod(hsvColor.h + step.h, 360.0);
    hsvColor.s = fmin(1.0, fmax(0.0, hsvColor.s + step.s));
    hsvColor.v = fmin(1.0, fmax(0.0, hsvColor.v + step.v));

    return color::toRGB(hsvColor);
}

bool GamePage::isVisible() const
{
    return true;
}

void GamePage::onBackspace()
{
    PageManager::shared().scrollUp();
}
