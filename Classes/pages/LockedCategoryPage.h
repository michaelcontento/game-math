#ifndef MATH_LOCKEDCATEGORYPAGE_H
#define MATH_LOCKEDCATEGORYPAGE_H

#include <avalon/payment.h>
#include "cocos2d.h"
#include "Page.h"
#include "../utils/config.h"

class LevelButton;

class LockedCategoryPage : public Page, public avalon::payment::ManagerDelegate
{
public:
    static LockedCategoryPage* create(void) = delete;
    static LockedCategoryPage* create(const int group);
    bool init(const int group);
    void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) override;

    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product) override;
    void onPurchaseFail(avalon::payment::Manager* const manager) override;
    void onTransactionStart(avalon::payment::Manager* const manager) override;
    void onTransactionEnd(avalon::payment::Manager* const manager) override;

private:
    int group = -1;
    const float middleOffsetY = 50 * config::getScaleFactor();
    cocos2d::DrawNode* draw = nullptr;
    cocos2d::DrawNode* boxes = nullptr;
    cocos2d::LabelTTF* subline = nullptr;
    bool doUnlock = false;

    static std::string lockedPrice;
    static bool lockedPriceFetched;
    static bool lockedPriceSomeoneFetching;

    void addHeadlineLabel();
    void addSublineLabel();
    void addCategoryBoxes();
    void addDescriptionLabel();
    void addPlayButton();
    void unlock();
    int getPaymentGroupId() const;
    std::string getHeadline() const;
    std::string getDescription() const;
    void updatePrice(const float dt);
    void fetchPrice(const float dt);
};

#endif // MATH_LOCKEDCATEGORYPAGE_H