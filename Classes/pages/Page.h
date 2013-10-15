#ifndef MATH_PAGE_H
#define MATH_PAGE_H

#include "cocos2d.h"

class PageManager;

class Page : public cocos2d::Layer
{
public:
    static Page* create(void) = delete;
    Page()
    : color(cocos2d::Color3B::WHITE)
    , background(nullptr)
    {};

    virtual ~Page() {};
    bool init() override;
    void visit() override;

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) {};

    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    virtual void onBackspace();

    void setBackground(const cocos2d::Color3B& color);
    cocos2d::Color3B getBackground() const;

    float getVisibleWidth() const;
    bool isVisible() const override;

private:
    cocos2d::Color3B color;
    cocos2d::LayerColor* background;
};

#endif // MATH_PAGE_H
