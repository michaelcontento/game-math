#include "PageManager.h"

#include "utils/config.h"
#include "pages/Page.h"

using namespace cocos2d;

PageManager* PageManager::instance = nullptr;

#pragma -
#pragma mark Page / scroll handling

void PageManager::add(const std::string& name, Page* const page)
{
    page->setPositionX(pages.size() * page->getContentSize().width);
    addChild(page);

    pages.push_back(std::make_pair(name, page));
    // FIX#1
    page->retain();
}

void PageManager::scrollTo(const std::string& name)
{
    scrollTo(name, config::getSnapAnimationDuration());
}

void PageManager::scrollTo(const std::string& name, const float duration)
{
    auto newZeroAlignedIndex = getPageIndex(name);
    auto index = 0;

    for (const auto& pair : pages) {
        auto page = pair.second;
        auto posX = (index - newZeroAlignedIndex) * page->getContentSize().width;

        scrollNodeTo(*page, {posX, 0}, duration);
        ++index;
    }
}

void PageManager::scrollDown(Page* const page)
{
    if (animationActive) {
        return;
    }

    if (pageScrollDown) {
        removeChild(pageScrollDown);
        pageScrollDown = nullptr;
    }

    pageScrollDown = page;
    addChild(pageScrollDown);

    // scroll all "menu pages"
    for (const auto& pair : pages) {
        auto page = pair.second;
        scrollNodeTo(
            *page,
            {page->getPositionX(), config::getFrameSize().height},
            config::getSnapAnimationDuration()
        );
    }

    // scroll the page down below
    pageScrollDown->setPositionY(config::getFrameSize().height * -1);
    scrollNodeTo(
        *pageScrollDown,
        {0, 0},
        config::getSnapAnimationDuration()
    );
}

void PageManager::scrollUp()
{
    if (!pageScrollDown || animationActive) {
        return;
    }

    // scroll all "menu pages"
    for (const auto& pair : pages) {
        auto page = pair.second;

        scrollNodeTo(
            *page,
            {page->getPositionX(), 0},
            config::getSnapAnimationDuration()
        );
    }

    // scroll the page down below
    scrollNodeTo(
        *pageScrollDown,
        {0, config::getFrameSize().height * -1},
        config::getSnapAnimationDuration(),
        [this]() { removeChild(pageScrollDown); pageScrollDown = nullptr; }
    );
}

void PageManager::scrollNodeTo(cocos2d::Node& node, const cocos2d::Point& newPosition, const float duration)
{
    scrollNodeTo(node, newPosition, duration, []() {});
}

void PageManager::scrollNodeTo(cocos2d::Node& node, const cocos2d::Point& newPosition, const float duration, std::function<void()> callback)
{
    node.stopActionByTag(TAG_ACTION_MOVE_BY);
    animationActive = true;

    if (duration > 0) {
        auto moveAction = Sequence::create(
            EaseInOut::create(MoveTo::create(duration, newPosition), 2),
            CallFunc::create(callback),
            DelayTime::create(config::getDelayAfterScrollAnimation()),
            CallFunc::create([this]() { animationActive = false; }),
            NULL
        );
        moveAction->setTag(TAG_ACTION_MOVE_BY);

        node.runAction(moveAction);
    } else {
        node.setPosition(newPosition);
        callback();
        animationActive = false;
    }
}

bool PageManager::isAnimationActive() const
{
    return animationActive;
}

#pragma -
#pragma mark Page helper

Page& PageManager::getPage(const std::string& name) const
{
    for (const auto& pair : pages) {
        if (pair.first == name) {
            return *pair.second;
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
    instance = nullptr;

    // FIX#1
    for (auto& pair : pages) {
        pair.second->release();
    }
}

bool PageManager::init()
{
    if (!Layer::init()) {
        return false;
    }

    setTouchEnabled(true);
    instance = this;

    return true;
}

PageManager& PageManager::shared()
{
    if (!instance) {
        throw new std::runtime_error("no PageManager instance ready yet");
    }
    
    return *instance;
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
    return !(animationActive || pageScrollDown);
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
        auto page = pair.second;
        page->setPositionX(page->getPositionX() + delta.x);
    }
}

void PageManager::snapPages()
{
    if (pages.empty() || animationActive) {
        return;
    }

    std::string activePage = "";
    int maxVisibleWidth = 0;

    for (const auto& pair : pages) {
        auto page = pair.second;

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
    scrollTo(activePage);
}