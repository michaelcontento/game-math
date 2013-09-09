#include "AnswerButton.h"

#include "../utils/fonts.h"
#include "../utils/config.h"
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
        ->addTargetedDelegate(this, -90, false);
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
    return !hasBeenUsed && isAnswerVisible();
}

bool AnswerButton::isAnswerVisible() const
{
    Object* it = nullptr;
    CCARRAY_FOREACH(container->getChildren(), it) {
        const auto child = dynamic_cast<NodeRGBA*>(it);
        if (!child) {
            continue;
        }

        return child->getOpacity() == 255;
    }

    return false;
}

void AnswerButton::ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!enabled || !ccTouchBegan(touch, event)) {
        return;
    }
    if (!boundingBox().containsPoint(touch->getLocation())) {
        return;
    }
    hasBeenUsed = true;
    
    indicatorLabelLeft->stopAllActions();
    indicatorLabelRight->stopAllActions();

    const auto posY = indicatorLabelRight->getPositionY();
    indicatorLabelLeft->runAction(EaseOut::create(MoveTo::create(0.15, {endPosLeft, posY}), 3));
    indicatorLabelRight->runAction(EaseOut::create(MoveTo::create(0.15, {endPosRight, posY}), 3));

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

    container = Node::create();
    addChild(container);

    return true;
}

cocos2d::LabelTTF* AnswerButton::addSubLabel(const std::string& text, const int fontSize)
{
    auto label = fonts::createLight(text, fontSize);
    container->addChild(label);
    
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setPositionX(lastContainerPos);

    const static auto padding = (1 * config::getScaleFactor());
    lastContainerPos += label->getContentSize().width + padding;
    
    return label;
};

bool AnswerButton::replaceSqrt(const std::string& text)
{
    auto work = text;
    auto pos = work.find("SQR#");
    if (pos == std::string::npos) {
        return false;
    }
    
    while (pos != std::string::npos) {
        // text before "SQR#"
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), 72);
        }
        work = work.substr(pos + 4);

        // square root sign
        const static auto signPadding = (3.5 * config::getScaleFactor());
        auto sign = addSubLabel("√", 63);
        sign->setPositionY(sign->getPositionY() + signPadding);

        // add the text to the next #
        const auto oldLastPos = lastContainerPos;
        pos = work.find("#");
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), 72);
        }
        work = work.substr(pos + 1);

        // extend the line of the square root sign
        const auto restPos = lastContainerPos;
        const static auto lineLeftOffset = 3 * config::getScaleFactor();
        const auto lineLenght = lastContainerPos - oldLastPos + lineLeftOffset;

        std::string lineStr = "_";
        auto line = addSubLabel(lineStr, 72);
        lastContainerPos = restPos;

        while (line->getContentSize().width <= lineLenght) {
            lineStr += "_";
            line->setString(lineStr.c_str());
        }
        line->setScaleX(lineLenght / line->getContentSize().width);

        line->setPositionX(oldLastPos - lineLeftOffset);
        line->setPositionY(62 * config::getScaleFactor());

        // prepare for the next loop iteration
        pos = work.find("SQR#");
    }
    
    if (work.length() > 0) {
        addSubLabel(work, 72);
    }
    return true;
}

bool AnswerButton::replaceFraction(const std::string& text)
{
    static const auto verticalOffset = 20 * config::getScaleFactor();
    static const auto lineExtra = 2 * config::getScaleFactor();
    auto work = text;
    auto pos = work.find("FRC#");
    if (pos == std::string::npos) {
        return false;
    }

    while (pos != std::string::npos) {
        // text before "FRC#"
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), 72);
        }
        work = work.substr(pos + 4);
        const auto oldLastPos = lastContainerPos;

        // get the upper text
        pos = work.find(";");
        auto upper = addSubLabel(work.substr(0, pos), 45);
        lastContainerPos = oldLastPos;
        upper->setPositionY(upper->getPositionY() + verticalOffset);
        work = work.substr(pos + 1);

        // get the upper text
        pos = work.find("#");
        auto lower = addSubLabel(work.substr(0, pos), 45);
        lastContainerPos = oldLastPos;
        lower->setPositionY(lower->getPositionY() - verticalOffset);
        work = work.substr(pos + 1);

        // align both upper and lower numbers
        const auto upperWidth = upper->getContentSize().width;
        const auto lowerWidth = lower->getContentSize().width;
        if (upperWidth > lowerWidth) {
            lower->setPositionX(lower->getPositionX() + (upperWidth - lowerWidth) / 2.0);
        } else if (lowerWidth > upperWidth) {
            upper->setPositionX(upper->getPositionX() + (lowerWidth - upperWidth) / 2.0);
        }

        // add the middle line
        const auto lineLenght = std::max(
            lower->getContentSize().width,
            upper->getContentSize().width
        ) + (lineExtra * 2);

        std::string lineStr = "_";
        auto line = addSubLabel(lineStr, 48);
        lastContainerPos = oldLastPos;

        while (line->getContentSize().width <= lineLenght) {
            lineStr += "_";
            line->setString(lineStr.c_str());
        }
        line->setScaleX(lineLenght / line->getContentSize().width);

        line->setPositionX(lastContainerPos - lineExtra);
        line->setPositionY(21  * config::getScaleFactor());

        // advance for the following characters
        lastContainerPos += lineLenght - (lineExtra * 2);

        // prepare for the next loop iteration
        pos = work.find("FRC#");
    }

    if (work.length() > 0) {
        addSubLabel(work, 72);
    }
    return true;
}

void AnswerButton::setAnswerString(const std::string& text)
{
    container->removeAllChildren();
    lastContainerPos = 0;

    hasBeenUsed = false;
    indicatorLabelLeft->setPositionX(startPosLeft);
    indicatorLabelRight->setPositionX(startPosRight);

    if (text.find("#") == std::string::npos) {
        addSubLabel(text, 72);
    } else {
        if (!replaceSqrt(text)) {
            if (!replaceFraction(text)) {
                addSubLabel(text, 72);
            }
        }
    }
    
    container->setContentSize(Size(lastContainerPos, 0));
    container->setAnchorPoint({0.5, 0});
    container->setPosition(Point(getContentSize() / 2));
}

void AnswerButton::setIsRight(const bool flag)
{
    isRight = flag;
}

bool AnswerButton::isRightAnswer() const
{
    return isRight;
}

void AnswerButton::fadeOutAnswer(const float duration)
{
    if (!isAnswerVisible()) {
        return;
    }
    
    Object* it = nullptr;
    CCARRAY_FOREACH(container->getChildren(), it) {
        const auto child = dynamic_cast<NodeRGBA*>(it);
        if (!child) {
            continue;
        }

        child->stopAllActions();
        child->runAction(FadeOut::create(duration));
    }

    // do not call stopAllActions() here because we can currently be in
    // the "slide in" animation!
    indicatorLabelLeft->runAction(FadeOut::create(duration));
    indicatorLabelRight->runAction(FadeOut::create(duration));
}

void AnswerButton::hideAnswer()
{
    Object* it = nullptr;
    CCARRAY_FOREACH(container->getChildren(), it) {
        const auto child = dynamic_cast<NodeRGBA*>(it);
        if (!child) {
            continue;
        }

        child->stopAllActions();
        child->setOpacity(0);
    }
    
    indicatorLabelLeft->stopAllActions();
    indicatorLabelLeft->setOpacity(0);
    indicatorLabelRight->stopAllActions();
    indicatorLabelRight->setOpacity(0);
}

void AnswerButton::showAnswer()
{
    Object* it = nullptr;
    CCARRAY_FOREACH(container->getChildren(), it) {
        const auto child = dynamic_cast<NodeRGBA*>(it);
        if (!child) {
            continue;
        }

        child->stopAllActions();
        child->setOpacity(255);
    }

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
    indicatorLabelLeft = fonts::createLight("", 72);
    indicatorLabelLeft->setColor(Color3B::WHITE);
    indicatorLabelLeft->setAnchorPoint({0.5, 0.5});
    indicatorLabelLeft->setPositionX(startPosLeft);
    indicatorLabelLeft->setPositionY(getContentSize().height / 2);
    addChild(indicatorLabelLeft);

    indicatorLabelRight = fonts::createLight("", 72);
    indicatorLabelRight->setColor(indicatorLabelLeft->getColor());
    indicatorLabelRight->setAnchorPoint(indicatorLabelLeft->getAnchorPoint());
    indicatorLabelRight->setPositionX(startPosRight);
    indicatorLabelRight->setPositionY(indicatorLabelLeft->getPositionY());
    addChild(indicatorLabelRight);
}