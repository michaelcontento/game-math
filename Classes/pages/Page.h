#ifndef MATH_PAGE_H
#define MATH_PAGE_H

#include <memory>
#include "cocos2d.h"

class PageManager;

class Page : public cocos2d::Layer
{
public:
    PageManager* manager = nullptr;

    Page* create(void) = delete;
    bool init() override;
    
    void registerWithTouchDispatcher() override;
    bool ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    virtual void onTouch(cocos2d::Touch* touch, cocos2d::Event* event) {};

    void setBackground(const cocos2d::ccColor3B& color);
    float getVisibleWidth() const;
    bool isVisible() const override;

private:
    constexpr static int TAG_BACKGROUND = 100;
};

#endif // MATH_PAGE_H