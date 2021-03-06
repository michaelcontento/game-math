#include "CategoryPage.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../PageManager.h"
#include "../buttons/LevelButton.h"

using namespace cocos2d;


CategoryPage* CategoryPage::create(const int group)
{
    CategoryPage* pRet = new CategoryPage();
    if (pRet && pRet->init(group)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CategoryPage::init(const int group)
{
    if (!Page::init()) {
        return false;
    }

    this->group = group;

    setBackground(config::getGroupColor(group));
    addHeadlineLabel();
    addLevelButtons();

    return true;
}

void CategoryPage::addHeadlineLabel()
{
    auto tapToPlay = fonts::createLight(config::getGroupHeadline(group).c_str(), 72, TextHAlignment::CENTER, TextVAlignment::CENTER);
    addChild(tapToPlay);

    tapToPlay->setColor(Color3B::WHITE);
    tapToPlay->setAnchorPoint({0.5, 0.5});
    tapToPlay->setPositionX(config::getFrameSize().width / 2);
    tapToPlay->setPositionY(config::getFrameSize().height - (75 * config::getScaleFactor()));
}

void CategoryPage::addLevelButtons()
{
    auto container = Node::create();
    addChild(container);

    auto spacingX = 15 * config::getScaleFactor();
    auto spacingY = 15 * std::max(config::getScaleFactor(), config::getScaleFactorHeight());
    auto containerSize = container->getContentSize();
    auto gridSize = Point(4, 4);

    for (int i = 0; i < (gridSize.x * gridSize.y); ++i) {
        auto btn = LevelButton::create(group, i + 1, *this);
        container->addChild(btn);
        levelButtons.push_back(btn);

        auto gridPos = Point(i % int(gridSize.x), (gridSize.y - 1) - int(i / gridSize.y));
        auto size = btn->getContentSize();
        btn->setPositionX(gridPos.x * (size.width  + spacingX) - spacingX);
        btn->setPositionY(gridPos.y * (size.height + spacingY) - spacingY);

        containerSize.width  = std::max(containerSize.width,  btn->getPositionX() + btn->getContentSize().width);
        containerSize.height = std::max(containerSize.height, btn->getPositionY() + btn->getContentSize().height);

        const auto halfSize = Point(btn->getContentSize() / 2);
        btn->setPosition(btn->getPosition() + halfSize);
        btn->setAnchorPoint({0.5, 0.5});
    }

    container->setContentSize(containerSize);
    container->setAnchorPoint({0.5, 0.5});
    container->setPosition(Point(config::getFrameSize() / 2));
}

void CategoryPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    std::for_each(
        levelButtons.begin(), levelButtons.end(),
        [&touch, &event](LevelButton* btn) { btn->onTouch(touch, event); }
    );
}

void CategoryPage::highlightNextLevel()
{
    if (lastBtnActionRunning) {
        return;
    }

    auto lastBtn = *--std::find_if(
        levelButtons.begin(), levelButtons.end(),
        [](LevelButton* btn) { return btn->isLocked(); }
    );

    lastBtn->stopAllActions();
    lastBtn->runAction(Sequence::create(
        CallFunc::create([this]() { lastBtnActionRunning = true; }),
        ScaleTo::create(0.2, 0.8),
        ScaleTo::create(0.2, 1.0),
        CallFunc::create([this]() { lastBtnActionRunning = false; }),
        NULL
    ));
}