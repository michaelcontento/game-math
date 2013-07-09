#ifndef MATH_PAGEMANAGER_H
#define MATH_PAGEMANAGER_H

#include <string>
#include <memory>
#include <list>
#include <utility>
#include <map>
#include "cocos2d.h"
#include "pages/Page.h"

class PageManager : public cocos2d::Layer
{
public:
    CREATE_FUNC(PageManager);
    bool init() override;
    ~PageManager();

    void registerWithTouchDispatcher() override;
    bool ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    bool hasTouchHandled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

    void add(const std::string& name, std::unique_ptr<Page> page);
    void scrollto(const std::string& name) const;
    void scrollto(const std::string& name, const float duration) const;
    void show(const std::string& name) const;

private:
    std::list<std::pair<std::string, std::unique_ptr<Page>>> pages {};
    std::map<int, bool> trackedTouches {};

    void handlePageScroll(const cocos2d::Point& delta);
    Page* getPage(const std::string& name) const;
    int getPageIndex(const std::string& name) const;
    void snapPages();
};

#endif // MATH_PAGEMANAGER_H