#ifndef MATH_PAGE_H
#define MATH_PAGE_H

#include "cocos2d.h"

class PageManager;

class Page : public cocos2d::Layer
{
public:
    static Page* create(void) = delete;
    bool init() override;
    void visit() override;

    void registerWithTouchDispatcher() override;
    bool ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    virtual void onTouch(cocos2d::Touch& touch, cocos2d::Event& event) {};

    void setBackground(const cocos2d::Color3B& color);
    const cocos2d::Color3B& getBackground() const;

    float getVisibleWidth() const;
    bool isVisible() const override;

private:
    cocos2d::Color3B color = cocos2d::Color3B::WHITE;
    cocos2d::LayerColor* background = nullptr;
};

#endif // MATH_PAGE_H