#include "PageManager.h"

using namespace cocos2d;

PageManager::~PageManager()
{
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

    return true;
}

void PageManager::add(const std::string& name, std::unique_ptr<Page> page)
{
    if (pages.empty()) {
        addChild(page.get());
    }

    // FIX#1
    page->retain();

    pages.push_back(std::make_pair(name, std::move(page)));
}

void PageManager::scrollto(const std::string& name)
{
}