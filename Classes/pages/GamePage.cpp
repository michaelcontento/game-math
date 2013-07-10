#include "GamePage.h"

#include "../utils/color.h"
#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../buttons/AnswerButton.h"
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

    addQuestion();
    addAnswerButtons();

    return true;
}

void GamePage::onTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
    PageManager::shared().scrollUp();
}

void GamePage::addQuestion()
{
    if (question) {
        throw new std::runtime_error("question already present");
    }

    auto label = fonts::createNormal("3", 96);
    label->setOpacity(0);
    configureAndAlignQuestionLabel(*label);

    question = label;
    addChild(question);

    question->runAction(Sequence::create(
       DelayTime::create(config::getQuestionStartDelay()),
       FadeOut::create(config::getQuestionFadeTime()),
       CallFunc::create([label]() { label->setString("2"); }),
       FadeOut::create(config::getQuestionFadeTime()),
       CallFunc::create([label]() { label->setString("1"); }),
       CallFunc::create([this]() { setNextQuestion(); }),
       NULL
    ));
}

void GamePage::setNextQuestion()
{
    if (!question) {
        throw new std::runtime_error("no question present -- call addQuestion() first");
    }

    auto label = dynamic_cast<LabelTTF*>(question);
    question->runAction(Sequence::create(
        FadeOut::create(config::getQuestionFadeTime()),
        CallFunc::create([label]() { label->setString("QUESTION"); label->setOpacity(255); }),
        NULL
    ));
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
        color = getNextAnswerButtonColor(color);

        container->addChild(btn);
        answerButtons.insert(btn);

        auto posY = (btn->getContentSize().height + spacing) * (answers - i);
        btn->setPosition({0, posY});
        
        containerSize.width  = std::max(containerSize.width,  btn->getPositionX() + btn->getContentSize().width);
        containerSize.height = std::max(containerSize.height, btn->getPositionY() + btn->getContentSize().height);
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