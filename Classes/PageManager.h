#ifndef MATH_PAGEMANAGER_H
#define MATH_PAGEMANAGER_H

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "cocos2d.h"
#include "cocos-ext.h"

class Page;

class PageManager : public cocos2d::Layer, public cocos2d::extension::ScrollViewDelegate
{
public:
    CREATE_FUNC(PageManager);
    bool init() override;
    ~PageManager();
    static PageManager& shared();

    void add(const std::string& name, Page* const page);
    void scrollTo(const std::string& name);
    void scrollTo(const std::string& name, const float duration);

    void scrollDown(Page* const page);
    void scrollUp();

    void scrollViewDidScroll(cocos2d::extension::ScrollView* view, bool stopped) override;
    void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override;

    bool hasControl();
    bool isPageVisible(const Page& page) const;

private:
    constexpr static int TAG_ACTION_MOVE_BY = 101;
    static PageManager* instance;
    cocos2d::extension::ScrollView* scrollView = nullptr;
    std::vector<std::pair<const std::string, Page* const>> pages {};
    std::unordered_map<int, bool> trackedTouches {};
    Page* pageScrollDown = nullptr;
    bool snapActive = false;

    void handlePageScroll(const cocos2d::Point& delta);
    Page& getPage(const std::string& name) const;
    int getPageIndex(const std::string& name) const;
    std::string getMostVisiblePageName() const;
};

#endif // MATH_PAGEMANAGER_H