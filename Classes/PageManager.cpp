#include "PageManager.h"

#include "utils/config.h"
#include "pages/Page.h"

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
        return;
    }

    scrollTo(getMostVisiblePageName());
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
        // FIX#3
        Page& nonConstPage = const_cast<Page&>(page);
        return scrollView->isNodeVisible(&nonConstPage);
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
}

void PageManager::scrollTo(const std::string& name)
{
    scrollTo(name, config::getSnapAnimationDuration());
}

void PageManager::scrollTo(const std::string& name, const float duration)
{
    float nonConstDuration = duration;
    auto index = getPageIndex(name);
    auto width = getPage(name).getContentSize().width;
    auto newPos = Point(index * width * -1, 0);

    snapActive = true;
    scrollView->unscheduleAllSelectors();
    if (duration > 0) {
        scrollView->setContentOffsetInDuration(newPos, nonConstDuration);
    } else {
        scrollView->setContentOffset(newPos, nonConstDuration);
    }
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
        config::getSnapAnimationDuration(),
        {0, config::getFrameSize().height}
    ));

    pageScrollDown->stopAllActions();
    pageScrollDown->setPositionY(config::getFrameSize().height * -1);
    pageScrollDown->runAction(MoveTo::create(
        config::getSnapAnimationDuration(),
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
        config::getSnapAnimationDuration(),
        {0, 0}
    ));

    pageScrollDown->stopAllActions();
    pageScrollDown->runAction(Sequence::create(
        MoveTo::create(config::getSnapAnimationDuration(), {0, config::getFrameSize().height * -1}),
        RemoveSelf::create(),
        NULL
    ));

    pageScrollDown = nullptr;
}

std::string PageManager::getMostVisiblePageName() const
{
    if (pages.empty()) {
        throw new std::runtime_error("there are no pages added yet");
    }
    
    std::string activePage {pages.begin()->first};
    float bestDistanceToZero {std::numeric_limits<float>::max()};
    float scrollViewOffset {scrollView->getContentOffset().x};

    for (const auto& pair : pages) {
        auto page = pair.second;

        float distance {fabsf(page->getPositionX() + scrollViewOffset)};

        if (distance < bestDistanceToZero) {
            bestDistanceToZero = distance;
            activePage = pair.first;
        }
    }

    return activePage;
}

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

PageManager::~PageManager()
{
    instance = nullptr;

    // FIX#1
    for (const auto& pair : pages) {
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

    scrollView = ScrollView::create(config::getFrameSize(), Node::create());
    scrollView->setDirection(kScrollViewDirectionHorizontal);
    scrollView->setDelegate(this);
    addChild(scrollView);

    return true;
}

PageManager& PageManager::shared()
{
    if (!instance) {
        throw new std::runtime_error("no PageManager instance ready yet");
    }
    
    return *instance;
}