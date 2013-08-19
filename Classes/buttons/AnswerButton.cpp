#include "AnswerButton.h"

#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../pages/Page.h"
#include "../pages/GamePage.h"
#include "../PageManager.h"

using namespace cocos2d;

void AnswerButton::onEnter()
{
    Node::onEnter();

    Director::getInstance()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, -90, true);
}

void AnswerButton::onExit()
{
    Director::getInstance()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    Node::onExit();
}

bool AnswerButton::ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return !hasBeenUsed && isAnswerVisible() && boundingBox().containsPoint(touch->getLocation());
}

bool AnswerButton::isAnswerVisible() const
{
    return label->getOpacity() == 255;
}

void AnswerButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!ccTouchBegan(touch, event)) {
        return;
    }
    hasBeenUsed = true;
    
    indicatorLabelLeft->stopAllActions();
    indicatorLabelRight->stopAllActions();

    const auto posY = indicatorLabelRight->getPositionY();
    indicatorLabelLeft->runAction(EaseOut::create(MoveTo::create(0.2, {endPosLeft, posY}), 3));
    indicatorLabelRight->runAction(EaseOut::create(MoveTo::create(0.2, {endPosRight, posY}), 3));

    if (isRight) {
        indicatorLabelLeft->setString("✓");
        indicatorLabelRight->setString("✓");
        page->answeredRight();
    } else {
        indicatorLabelLeft->setString("✗");
        indicatorLabelRight->setString("✗");
        page->answeredWrong();
    }
}

AnswerButton* AnswerButton::create(GamePage& page, const cocos2d::Color3B& color)
{
    AnswerButton* pRet = new AnswerButton();
    if (pRet && pRet->init(page, color)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool AnswerButton::init(GamePage& page, const cocos2d::Color3B& color)
{
    if (!Node::init()) {
        return false;
    }

    this->page = &page;

    setContentSize(config::getAnswerButtonSize());
    addBackground(color);
    addLabels();

    return true;
}

void AnswerButton::setAnswerString(const std::string& text)
{
    hasBeenUsed = false;
    label->setString(text.c_str());
    indicatorLabelLeft->setPositionX(startPosLeft);
    indicatorLabelRight->setPositionX(startPosRight);
}

void AnswerButton::setIsRight(const bool flag)
{
    isRight = flag;
}

void AnswerButton::fadeOutAnswer(const float duration)
{
    label->stopAllActions();
    label->runAction(FadeOut::create(duration));

    // do not call stopAllActions() here because we can currently be in
    // the "slide in" animation!
    indicatorLabelLeft->runAction(FadeOut::create(duration));
    indicatorLabelRight->runAction(FadeOut::create(duration));
}

void AnswerButton::hideAnswer()
{
    label->stopAllActions();
    label->setOpacity(0);

    indicatorLabelLeft->stopAllActions();
    indicatorLabelLeft->setOpacity(0);
    indicatorLabelRight->stopAllActions();
    indicatorLabelRight->setOpacity(0);
}

void AnswerButton::showAnswer()
{
    label->stopAllActions();
    label->setOpacity(255);

    indicatorLabelLeft->stopAllActions();
    indicatorLabelLeft->setOpacity(255);
    indicatorLabelRight->stopAllActions();
    indicatorLabelRight->setOpacity(255);
}

void AnswerButton::addBackground(const cocos2d::Color3B& color)
{
    auto draw = DrawNode::create();
    addChild(draw);

    auto size = getContentSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};

    draw->drawPolygon(verts, 4, color::toRGBA(color), 0, {});
}

void AnswerButton::addLabels()
{
    label = fonts::createLight("", 72);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setPosition(Point(getContentSize() / 2));
    addChild(label);

    indicatorLabelLeft = fonts::createLight("", 72);
    indicatorLabelLeft->setColor(Color3B::WHITE);
    indicatorLabelLeft->setAnchorPoint({0.5, 0.5});
    indicatorLabelLeft->setPositionX(startPosLeft);
    indicatorLabelLeft->setPositionY(label->getPositionY());
    addChild(indicatorLabelLeft);

    indicatorLabelRight = fonts::createLight("", 72);
    indicatorLabelRight->setColor(indicatorLabelLeft->getColor());
    indicatorLabelRight->setAnchorPoint(indicatorLabelLeft->getAnchorPoint());
    indicatorLabelRight->setPositionX(startPosRight);
    indicatorLabelRight->setPositionY(indicatorLabelLeft->getPositionY());
    addChild(indicatorLabelRight);
}