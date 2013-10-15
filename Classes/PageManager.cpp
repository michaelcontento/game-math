#include "PageManager.h"

#include <boost/foreach.hpp>
#include <avalon/Appirater.h>
#include <avalon/ads/Manager.h>
#include "utils/config.h"
#include "utils/user.h"
#include "pages/Page.h"
#include "pages/SettingsPage.h"
#include "Indicator.h"

using namespace cocos2d;
using namespace cocos2d::extension;

PageManager* PageManager::instance = nullptr;

void PageManager::scrollViewDidScroll(cocos2d::extension::ScrollView* view, bool stopped)
{
    if (pages.empty() || !stopped) {
        return;
    }

    if (snapActive) {
        snapActive = false;
        indicator->updateOpacity();
        return;
    }

    auto offsetX = fabsf(scrollView->getContentOffset().x - config::getFrameSize().width);
    if (offsetX > scrollView->getContentSize().width) {
        // right side bouncing
    } else if (offsetX < config::getFrameSize().width) {
        // left side bouncing
    } else {
        scrollTo(getMostVisiblePageName());
    }
}

void PageManager::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
}

bool PageManager::hasControl()
{
    return scrollView->isTouchMoved() || snapActive;
}

bool PageManager::isPageVisible(const Page& page) const
{
    // FIX#2
    if (pageScrollDown && scrollView->getPositionY() >= config::getFrameSize().height) {
        auto nonConstPagePtr = const_cast<Page*>(&page);
        return (nonConstPagePtr == pageScrollDown);
    } else {
        auto offset = scrollView->getContentOffset();
        auto pos = page.getPosition();
        auto size = page.getContentSize();
        auto realPos = pos + offset;

        if (realPos.x + size.width <= 0) {
            return false;
        }
        if (realPos.x >= config::getFrameSize().width) {
            return false;
        }
        return true;
    }
}

void PageManager::add(const std::string& name, Page* const page)
{
    page->setPositionX(pages.size() * page->getContentSize().width);
    scrollView->addChild(page);

    scrollView->setContentSize({
        page->getPositionX() + page->getContentSize().width,
        scrollView->getContentSize().height
    });

    pages.push_back(std::make_pair(name, page));
    // FIX#1
    page->retain();

    indicator->addDot();
}

void PageManager::replacePage(Page& oldPage, const std::string& nameA, Page* const pageA, const std::string& nameB, Page* const pageB)
{
    auto pos = std::find_if(
        pages.begin(),
        pages.end(),
        [&oldPage](const NamedPage& row) { return row.second == &oldPage; }
    );
    if (pos == pages.end()) {
        return;
    }

    pos = pages.erase(pos);
    oldPage.removeFromParent();
    oldPage.release();

    pos = pages.insert(pos, std::make_pair(nameB, pageB));
    scrollView->addChild(pageA);
    pageA->retain();

    pos = pages.insert(pos, std::make_pair(nameA, pageA));
    scrollView->addChild(pageB);
    pageB->retain();

    updateScrollViewPositions();
    indicator->addDot();
}

void PageManager::updateScrollViewPositions()
{
    auto scrollViewHeight = scrollView->getContentSize().height;
    scrollView->setContentSize({0, scrollViewHeight});

    int index = 0;
    BOOST_FOREACH (auto& pair, pages) {
        auto page = pair.second;
        auto pageWidth = page->getContentSize().width;

        page->setPositionX(index * pageWidth);
        scrollView->setContentSize({page->getPositionX() + pageWidth, scrollViewHeight});

        ++index;
    }
}

void PageManager::scrollTo(const std::string& name)
{
    scrollTo(name, config::getSnapAnimationDuration());
}

void PageManager::scrollTo(const std::string& name, const float duration)
{
    float nonConstDuration = duration;
    auto index = getPageIndex(name);
    auto width = getPage(name)->getContentSize().width;
    auto newPos = Point(index * width * -1, 0);

    snapActive = true;
    scrollView->unscheduleAllSelectors();
    if (duration > 0) {
        scrollView->setContentOffsetInDuration(newPos, nonConstDuration);
    } else {
        scrollView->setContentOffset(newPos, nonConstDuration);
    }
}

void PageManager::scrollHome()
{
    if (pageScrollDown) {
        return;
    }

    static const auto mainIndex = getPageIndex("main");
    static const auto mainWidth = getPage("main")->getContentSize().width;
    static const auto newPos = mainIndex * mainWidth * -1;

    const auto scrollDiff = std::abs(scrollView->getContentOffset().x - newPos);
    const auto duration = (config::getSnapAnimationDuration() / mainWidth) * scrollDiff;

    snapActive = true;
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffsetInDuration(Point(newPos, 0), duration);
}

void PageManager::scrollDown(Page* const page)
{
    if (hasControl() || pageScrollDown) {
        return;
    }

    pageScrollDown = page;
    addChild(pageScrollDown);

    scrollView->stopAllActions();
    scrollView->runAction(MoveTo::create(
        config::getScrollDownDuration(),
        {0, config::getFrameSize().height}
    ));

    indicator->stopAllActions();
    indicator->runAction(MoveTo::create(
        config::getScrollDownDuration(),
        {indicator->getPositionX(), config::getFrameSize().height}
    ));

    pageScrollDown->stopAllActions();
    pageScrollDown->setPositionY(config::getFrameSize().height * -1);
    pageScrollDown->runAction(MoveTo::create(
        config::getScrollDownDuration(),
        {0, 0}
    ));
}

void PageManager::scrollUp()
{
    if (!pageScrollDown) {
        return;
    }

    scrollView->stopAllActions();
    scrollView->runAction(MoveTo::create(
        config::getScrollDownDuration(),
        {0, 0}
    ));

    indicator->stopAllActions();
    indicator->runAction(MoveTo::create(
        config::getScrollDownDuration(),
        {indicator->getPositionX(), 0}
    ));

    pageScrollDown->stopAllActions();
    pageScrollDown->runAction(Sequence::create(
        MoveTo::create(config::getScrollDownDuration(), {0, config::getFrameSize().height * -1}),
        RemoveSelf::create(),
        CallFunc::create([]() {
            avalon::Appirater::getInstance()->showIfNeeded();
        }),
        NULL
    ));

    pageScrollDown = nullptr;

    if (user::hasAdsEnabled()) {
        avalon::ads::Manager::showFullscreenAd();

        auto settingsPage = dynamic_cast<SettingsPage*>(getPage("settings"));
        if (settingsPage) {
            settingsPage->showRemoveAds();
        }
    }
}

std::string PageManager::getMostVisiblePageName() const
{
    if (pages.empty()) {
        throw new std::runtime_error("there are no pages added yet");
    }

    std::string activePage = pages.begin()->first;
    float bestDistanceToZero {std::numeric_limits<float>::max()};
    float scrollViewOffset {scrollView->getContentOffset().x};

    BOOST_FOREACH (auto& pair, pages) {
        auto page = pair.second;

        float distance {fabsf(page->getPositionX() + scrollViewOffset)};

        if (distance < bestDistanceToZero) {
            bestDistanceToZero = distance;
            activePage = pair.first;
        }
    }

    return activePage;
}

Page* PageManager::getPage(const std::string& name) const
{
    BOOST_FOREACH (auto& pair, pages) {
        if (pair.first == name) {
            return pair.second;
        }
    }

    throw new std::out_of_range("invalid page name given" + name);
}

int PageManager::getPageIndex(const std::string& name) const
{
    int index = 0;
    BOOST_FOREACH (auto& pair, pages) {
        if (pair.first == name) {
            return index;
        }
        ++index;
    }

    throw new std::out_of_range("invalid page name given" + name);
}

PageManager::~PageManager()
{
    instance = nullptr;

    // FIX#1
    BOOST_FOREACH (auto& pair, pages) {
        pair.second->release();
    }
}

bool PageManager::init()
{
    if (!Layer::init()) {
        return false;
    }

    instance = this;

    scrollView = ScrollView::create(config::getFrameSize(), Node::create());
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    scrollView->setDelegate(this);
    scrollView->minMoveDistance = 20 * config::getScaleFactor();
    addChild(scrollView);

    indicator = Indicator::create();
    indicator->setCascadeOpacityEnabled(true);
    indicator->setOpacity(0);
    indicator->setAnchorPoint({0.5, 0});
    indicator->setPositionX(config::getFrameSize().width * 0.5);
    indicator->setZOrder(100);
    addChild(indicator);

    return true;
}

PageManager& PageManager::shared()
{
    if (!instance) {
        throw new std::runtime_error("no PageManager instance ready yet");
    }

    return *instance;
}


int PageManager::getCurrentPageIndex() const
{
    return abs(static_cast<int>(scrollView->getContentOffset().x / config::getFrameSize().width));
}
