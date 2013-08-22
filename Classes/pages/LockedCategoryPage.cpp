#include "LockedCategoryPage.h"

#include "../utils/fonts.h"
#include "../utils/color.h"
#include "../utils/user.h"
#include "../utils/helper.h"
#include "../PageManager.h"
#include "CategoryPage.h"

using namespace cocos2d;
using namespace avalon;

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
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);

    // positioning
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY(config::getFrameSize().height - (75 * config::getScaleFactor()));
}

void LockedCategoryPage::addSublineLabel()
{
    auto label = fonts::createLight("2 categories, 32 levels", 36);
    addChild(label);

    // color
    label->setColor(Color3B::BLACK);

    // alignment
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);

    // positioning
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY((config::getFrameSize().height / 4 * 3) + middleOffsetY);
}

std::string LockedCategoryPage::getHeadline() const
{
    if        (group == 3) {
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

std::string LockedCategoryPage::getDescription() const
{
    if        (group == 3) {
        return "these should be possible too, right?";
    } else if (group == 5) {
        return "some fancy words. bla bla yadda yadda";
    } else if (group == 7) {
        return "also ich würde das ja direkt kaufen\nso günstig ist bildung selten!";
    } else if (group == 9) {
        return "hallo sena! wie findest du die text\nhier denn so? ich bin unkreativ. punkt.";
    } else {
        throw new std::runtime_error("invalig group given");
    }
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
    auto labelA = fonts::createLight(config::getGroupHeadline(group).c_str(), 68);
    labelA->setColor(Color3B::WHITE);
    labelA->setAnchorPoint({0.5, 0.5});
    labelA->setHorizontalAlignment(TextHAlignment::CENTER);
    labelA->setVerticalAlignment(TextVAlignment::CENTER);
    labelA->setPositionX(size.width / 2);
    labelA->setPositionY(size.height + spacing + (size.height / 2));
    boxes->addChild(labelA);

    // lower label
    auto labelB = fonts::createLight(config::getGroupHeadline(group + 1).c_str(), 68);
    labelB->setColor(Color3B::WHITE);
    labelB->setAnchorPoint({0.5, 0.5});
    labelB->setHorizontalAlignment(TextHAlignment::CENTER);
    labelB->setVerticalAlignment(TextVAlignment::CENTER);
    labelB->setPositionX(size.width / 2);
    labelB->setPositionY(size.height / 2);
    boxes->addChild(labelB);

    // alignment
    boxes->setContentSize({size.width, size.height * 2 + spacing});
    boxes->setAnchorPoint({0.5, 0.5});
    boxes->setPosition(Point(config::getFrameSize() / 2));
    boxes->setPositionY(boxes->getPositionY() + middleOffsetY);
}

void LockedCategoryPage::addDescriptionLabel()
{
    auto label = fonts::createLight(getDescription().c_str(), 36);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setPositionX(config::getFrameSize().width / 2);
    label->setPositionY((config::getFrameSize().height / 4 * 1) + middleOffsetY);
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

    const auto color = color::toRGBA(Color3B(171, 168, 171));
    draw->drawPolygon(verts, 4, color, 0, {});

    draw->setContentSize(size);

    // == LABEL

    auto label = fonts::createLight("PLAY!", 72);
    label->setColor(Color3B::WHITE);
    label->setAnchorPoint({0.5, 0.5});
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setPosition(Point(size / 2));
    draw->addChild(label);
}

void LockedCategoryPage::onTouch(cocos2d::Touch& touch, cocos2d::Event& event)
{
    if (!draw->getBoundingBox().containsPoint(touch.getLocation())) {
        return; // not the play button touched
    }

    if (!user::isLevelGroupLocked(getPaymentGroupId())) {
        unlock();
        return; // skip early if possible
    }

    auto payment = payment::Loader::globalManager;
    payment->delegate = this;

    if (!helper::paymentAvailableCheck(payment.get())) {
        return; // payment not available
    }

    const auto key = std::string("pack.") + std::to_string(getPaymentGroupId());
    if (!payment->hasProduct(key.c_str())) {
        return; // invalid product id
    }

    payment->getProduct(key.c_str())->purchase();
}

int LockedCategoryPage::getPaymentGroupId() const
{
    return std::ceil(group / 2.f) - 1;
}

void LockedCategoryPage::onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product)
{
    user::setLevelGroupLocked(getPaymentGroupId(), false);
    unlock();
}

void LockedCategoryPage::unlock()
{
    std::string name = "category-";
    PageManager::shared().replacePage(
        *this,
        name + std::to_string(group + 0),
        CategoryPage::create(group + 0),
        name + std::to_string(group + 1),
        CategoryPage::create(group + 1)
    );
}

void LockedCategoryPage::onPurchaseFail(avalon::payment::Manager* const manager)
{
}

void LockedCategoryPage::onTransactionStart(avalon::payment::Manager* const manager)
{
}

void LockedCategoryPage::onTransactionEnd(avalon::payment::Manager* const manager)
{
}