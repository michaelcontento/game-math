#ifndef MATH_PAGEMANAGER_H
#define MATH_PAGEMANAGER_H

#include <string>
#include <list>
#include <utility>
#include <unordered_map>
#include "cocos2d.h"
#include "cocos-ext.h"

class Page;
class Indicator;

class PageManager : public cocos2d::Layer, public cocos2d::extension::ScrollViewDelegate
{
public:
    PageManager()
    : indicator(nullptr)
    , scrollView(nullptr)
    , pageScrollDown(nullptr)
    , snapActive(false)
    {};
    
    Indicator* indicator;
    
    CREATE_FUNC(PageManager);
    bool init() override;
    ~PageManager();
    static PageManager& shared();

    void add(const std::string& name, Page* const page);
    Page* getPage(const std::string& name) const;

    void scrollTo(const std::string& name);
    void scrollTo(const std::string& name, const float duration);
    void scrollHome();

    void scrollDown(Page* const page);
    void scrollUp();

    void scrollViewDidScroll(cocos2d::extension::ScrollView* view, bool stopped) override;
    void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override;

    bool hasControl();
    bool isPageVisible(const Page& page) const;

    int getCurrentPageIndex() const;

    void replacePage(Page& oldPage, const std::string& nameA, Page* const pageA, const std::string& nameB, Page* const pageB);

private:
    typedef std::pair<const std::string, Page* const> NamedPage;
    constexpr static int TAG_ACTION_MOVE_BY = 101;
    static PageManager* instance;
    cocos2d::extension::ScrollView* scrollView;
    std::list<NamedPage> pages;
    std::unordered_map<int, bool> trackedTouches;
    Page* pageScrollDown;
    bool snapActive;

    void handlePageScroll(const cocos2d::Point& delta);
    int getPageIndex(const std::string& name) const;
    std::string getMostVisiblePageName() const;
    void updateScrollViewPositions();
};

#endif // MATH_PAGEMANAGER_H
