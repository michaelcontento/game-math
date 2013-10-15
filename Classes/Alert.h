#ifndef MATH_ALERT_H
#define MATH_ALERT_H

#include <functional>
#include <vector>
#include "cocos2d.h"

class Alert : public cocos2d::Layer
{
public:
    Alert()
    : visible(false)
    , touchable(false)
    , closeOnTap(true)
    , buttonContainer(nullptr)
    , timeoutSecs(0)
    , desc(nullptr)
    , draw(nullptr)
    , tap(nullptr)
    {};

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

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

private:
    bool visible;
    bool touchable;
    bool closeOnTap;
    typedef std::function< void()> ButtonCallback;
    typedef std::pair<Node*, ButtonCallback> ButtonsPair;
    std::vector<ButtonsPair> buttons;
    std::function<void ()> callback;
    std::function<void ()> buttonCb;
    std::function<void ()> timeoutCb;
    Node* buttonContainer;
    float timeoutSecs;
    cocos2d::LabelTTF* desc;
    cocos2d::DrawNode* draw;
    cocos2d::LabelTTF* tap;
};

#endif // MATH_ALERT_H
