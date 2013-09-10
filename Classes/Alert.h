#ifndef MATH_ALERT_H
#define MATH_ALERT_H

#include <functional>
#include <vector>
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
    void hide(const bool instant = false);

    void setTimeout(const float secs, std::function<void ()> callback);
    void onTick(const float dt);

    void addButton(const std::string& description, std::function<void ()> callback);

    bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void ccTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    bool visible = false;
    bool touchable = false;
    bool closeOnTap = true;
    std::vector<std::pair<Node*, std::function< void()>>> buttons;
    std::function<void ()> callback;
    std::function<void ()> buttonCb;
    std::function<void ()> timeoutCb;
    Node* buttonContainer = nullptr;
    float timeoutSecs = 0;
    cocos2d::LabelTTF* desc = nullptr;
    cocos2d::DrawNode* draw = nullptr;
    cocos2d::LabelTTF* tap = nullptr;
};

#endif // MATH_ALERT_H
