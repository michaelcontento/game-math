#ifndef MATH_ALERT_H
#define MATH_ALERT_H

#include <functional>
#include "cocos2d.h"

class Alert : public cocos2d::Layer
{
public:
    CREATE_FUNC(Alert);

    bool init() override;
    void visit() override;

    void setDescription(const std::string& description);
    void enableCloseOnTap(const bool flag = true);

    void show(std::function<void ()> callback, const bool instant = false);
    void hide();

    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    bool visible = false;
    bool touchable = false;
    bool closeOnTap = true;
    std::function<void ()> callback;
    cocos2d::LabelTTF* desc = nullptr;
    cocos2d::DrawNode* draw = nullptr;
    cocos2d::LabelTTF* tap = nullptr;
};

#endif // MATH_ALERT_H
