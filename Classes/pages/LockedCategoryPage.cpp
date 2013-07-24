#include "LockedCategoryPage.h"

#include "../utils/config.h"
#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../PageManager.h"
#include "CategoryPage.h"

using namespace cocos2d;

LockedCategoryPage* LockedCategoryPage::create(const int group)
{
    LockedCategoryPage* pRet = new LockedCategoryPage();
    if (pRet && pRet->init(group)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LockedCategoryPage::init(const int group)
{
    if (!Page::init()) {
        return false;
    }

    this->group = group;
    setBackground(Color3B::WHITE);

    addHeadlineLabel();
    addSublineLabel();
    addCategoryBoxes();
    addDescriptionLabel();
    addPlayButton();

    return true;
}

void LockedCategoryPage::addHeadlineLabel()
{
    auto label = fonts::createNormal(getHeadline().c_str(), 72);
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY(config::getFrameSize().height - (50 * config::getScaleFactor()));
}

const std::string LockedCategoryPage::getHeadline() const
{
    if (group == 3) {
        return "PACK ONE";
    } else if (group == 5) {
        return "PACK TWO";
    } else if (group == 7) {
        return "PACK THREE";
    } else if (group == 9) {
        return "PACK FOUR";
    } else {
        throw new std::runtime_error("invalig group given");
    }
}

void LockedCategoryPage::addSublineLabel()
{
    auto label = fonts::createLight("2 categories, 32 levels", 36);
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);

    // positioning
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY(config::getFrameSize().height / 4 * 3);
}

void LockedCategoryPage::addCategoryBoxes()
{
    auto boxes = DrawNode::create();
    addChild(boxes);

    // keep things "readable"
    Size size = {config::getFrameSize().width * 0.75, config::getAnswerButtonSize().height * 1.25};
    float spacing = 50 * config::getScaleFactor();

    // upper rectangle
    Point vertsA[] = {{0, size.height + spacing}, {0, size.height * 2 + spacing}, {size.width, size.height * 2 + spacing}, {size.width, size.height + spacing}};
    auto colorA = config::getGroupColor(group);
    boxes->drawPolygon(vertsA, 4, color::toRGBA(colorA), 0, {});

    // lower rectangle
    Point vertsB[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};
    auto colorB = config::getGroupColor(group + 1);
    boxes->drawPolygon(vertsB, 4, color::toRGBA(colorB), 0, {});

    // upper label
    auto labelA = fonts::createLight(config::getGroupHeadline(group).c_str(), 58);
    labelA->setColor(Color3B::WHITE);
    labelA->setAnchorPoint({0.5, 0.5});
    labelA->setHorizontalAlignment(kTextAlignmentCenter);
    labelA->setVerticalAlignment(kVerticalTextAlignmentCenter);
    labelA->setPositionX(size.width / 2);
    labelA->setPositionY(size.height + spacing + (size.height / 2));
    boxes->addChild(labelA);

    // lower label
    auto labelB = fonts::createLight(config::getGroupHeadline(group + 1).c_str(), 58);
    labelB->setColor(Color3B::WHITE);
    labelB->setAnchorPoint({0.5, 0.5});
    labelB->setHorizontalAlignment(kTextAlignmentCenter);
    labelB->setVerticalAlignment(kVerticalTextAlignmentCenter);
    labelB->setPositionX(size.width / 2);
    labelB->setPositionY(size.height / 2);
    boxes->addChild(labelB);

    // alignment
    boxes->setContentSize({size.width, size.height * 2 + spacing});
    boxes->setAnchorPoint({0.5, 0.5});
    boxes->setPosition(config::getFrameSize() / 2);
}

void LockedCategoryPage::addDescriptionLabel()
{
    auto label = fonts::createLight("these should be possible too, right?", 36);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY(config::getFrameSize().height / 4 * 1);
    addChild(label);
}

void LockedCategoryPage::addPlayButton()
{
    // == BACKGROUND

    draw = DrawNode::create();
    draw->setPositionY(config::getAnswerButtonPadding() * 2);
    addChild(draw);

    Size size = config::getAnswerButtonSize();
    Point verts[] = {{0, 0}, {0, size.height}, {size.width, size.height}, {size.width, 0}};

    Color4F color = {0.4, 0.4, 0.4, 1};
    draw->drawPolygon(verts, 4, color, 0, {});

    draw->setContentSize(size);

    // == LABEL

    auto label = fonts::createLight("PLAY!", 72);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(kTextAlignmentCenter);
    label->setVerticalAlignment(kVerticalTextAlignmentCenter);
    label->setPosition(size / 2);
    draw->addChild(label);
}

void LockedCategoryPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    if (!draw->getBoundingBox().containsPoint(touch.getLocation())) {
        return;
    }

    std::string name = "category-";
    PageManager::shared().replacePage(
        *this,
        name + std::to_string(group + 0),
        CategoryPage::create(group + 0),
        name + std::to_string(group + 1),
        CategoryPage::create(group + 1)
    );
}