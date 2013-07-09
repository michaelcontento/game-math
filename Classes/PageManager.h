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

    void add(const std::string& name, Page* const page);
    void scrollto(const std::string& name);
    void scrollto(const std::string& name, const float duration);

    void scrolldown(Page* const page);
    void scrollup();

private:
    std::list<std::pair<std::string, Page*>> pages;
    std::map<int, bool> trackedTouches;
    Page* pageScrollDown = nullptr;
    constexpr static int TAG_ACTION_MOVE_BY = 101;

    void handlePageScroll(const cocos2d::Point& delta);
    Page& getPage(const std::string& name) const;
    int getPageIndex(const std::string& name) const;
    void snapPages();
    void scrollNodeTo(cocos2d::Node& node, const cocos2d::Point& newPosition, const float duration, std::function<void()> callback = []() {});

};

#endif // MATH_PAGEMANAGER_H