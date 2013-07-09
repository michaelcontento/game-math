#include "PageManager.h"

#include "utils/config.h"

using namespace cocos2d;

void PageManager::add(const std::string& name, std::unique_ptr<Page> page)
{
    page->manager = this;
    page->setPositionX(pages.size() * config::getFrameSize().width);
    addChild(page.get());

    // FIX#1
    page->retain();

    pages.push_back(std::make_pair(name, std::move(page)));
}

void PageManager::scrollto(const std::string& name) const
{
    scrollto(name, config::getSnapAnimationDuration());
}

void PageManager::scrollto(const std::string& name, const float duration) const
{
    auto scrollAmount = getPage(name)->getPositionX() * -1;

    for (const auto& pair : pages) {
        auto moveAction = MoveBy::create(duration, {scrollAmount, 0});
        pair.second.get()->runAction(EaseInOut::create(moveAction, 2));
    }
}

void PageManager::show(const std::string& name) const
{
    scrollto(name, 0);
}

Page* PageManager::getPage(const std::string& name) const
{
    for (const auto& pair : pages) {
        if (pair.first == name) {
            return pair.second.get();
        }
    }

    throw new std::out_of_range("invalid page name given" + name);
}

int PageManager::getPageIndex(const std::string& name) const
{
    int index = 0;
    for (const auto& pair : pages) {
        if (pair.first == name) {
            return index;
        }
        ++index;
    }

    throw new std::out_of_range("invalid page name given" + name);
}

#pragma -
#pragma mark Lifetime

PageManager::~PageManager()
{
    // FIX#1
    for (auto& pair : pages) {
        removeChild(pair.second.get());
        pair.second->release();
    }
}

bool PageManager::init()
{
    if (!Layer::init()) {
        return false;
    }

    setTouchEnabled(true);

    return true;
}

#pragma -
#pragma mark Touch handling

void PageManager::registerWithTouchDispatcher()
{
    Director::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, false);
}

bool PageManager::ccTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    return true;
}

void PageManager::ccTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    // touch captured, handle page scrolling
    if (hasTouchHandled(pTouch, pEvent)) {
        handlePageScroll(pTouch->getDelta());
        return;
    }

    // we've previously decided to ignore this touch
    if (trackedTouches.count(pTouch->getID()) > 0) {
        return;
    }

    auto startDelta = pTouch->getLocation() - pTouch->getStartLocation();

    // detect if we should ignore this touch for page scrolling
    if (fabs(startDelta.y) >= config::getSwipeMovementMaxY()) {
        trackedTouches.insert(std::make_pair(pTouch->getID(), false));
        return;
    }

    // detect if we should enable page scrolling
    if (fabs(startDelta.x) >= config::getSwipeMovementMinX()) {
        trackedTouches.insert(std::make_pair(pTouch->getID(), true));
    }
}

void PageManager::ccTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    if (hasTouchHandled(pTouch, pEvent)) {
        snapPages();
    }
    
    trackedTouches.erase(pTouch->getID());
}

bool PageManager::hasTouchHandled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    if (trackedTouches.count(pTouch->getID()) == 0) {
        return false;
    } else {
        return trackedTouches.at(pTouch->getID());
    }
}

void PageManager::handlePageScroll(const cocos2d::Point& delta)
{
    for (auto& pair : pages) {
        auto page = pair.second.get();
        page->setPositionX(page->getPositionX() + delta.x);
    }
}

void PageManager::snapPages()
{
    if (pages.empty()) {
        return;
    }

    std::string activePage = "";
    int maxVisibleWidth = 0;

    for (const auto& pair : pages) {
        auto page = pair.second.get();

        // fallback #1: use the first page that's right from pos 0
        if (activePage.empty()) {
            auto pageRightPos = page->getPositionX() + page->getContentSize().width;
            if (pageRightPos >= 0) {
                activePage = pair.first;
            }
        }

        if (!page->isVisible()) {
            continue;
        }

        // use the page with the most visible width / pixels
        auto currentVisibleWidth = page->getVisibleWidth();
        if (currentVisibleWidth > maxVisibleWidth) {
            maxVisibleWidth = currentVisibleWidth;
            activePage = pair.first;
        }
    }

    // fallback #2: simply use to the last page available
    if (activePage.empty()) {
        activePage = (--pages.end())->first;
    }

    // kickoff the final scrolling animation
    scrollto(activePage);
}