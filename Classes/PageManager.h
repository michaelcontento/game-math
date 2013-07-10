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

    static PageManager& shared();

    void registerWithTouchDispatcher() override;
    bool ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    void ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
    bool hasTouchHandled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

    void add(const std::string& name, Page* const page);
    void scrollTo(const std::string& name);
    void scrollTo(const std::string& name, const float duration);

    void scrollDown(Page* const page);
    void scrollUp();

    bool isAnimationActive() const;

private:
    std::list<std::pair<std::string, Page*>> pages;
    std::map<int, bool> trackedTouches;
    Page* pageScrollDown = nullptr;
    bool animationActive = false;
    static PageManager* instance;
    constexpr static int TAG_ACTION_MOVE_BY = 101;

    void handlePageScroll(const cocos2d::Point& delta);
    Page& getPage(const std::string& name) const;
    int getPageIndex(const std::string& name) const;
    void snapPages();
    void scrollNodeTo(cocos2d::Node& node, const cocos2d::Point& newPosition, const float duration);
    void scrollNodeTo(cocos2d::Node& node, const cocos2d::Point& newPosition, const float duration, std::function<void()> callback);
};

#endif // MATH_PAGEMANAGER_H