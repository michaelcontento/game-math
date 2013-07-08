#ifndef MATH_PAGEMANAGER_H
#define MATH_PAGEMANAGER_H

#include <string>
#include <memory>
#include <list>
#include <utility>
#include "cocos2d.h"
#include "pages/Page.h"

class PageManager : public cocos2d::Layer
{
public:
    CREATE_FUNC(PageManager);
    bool init() override;
    ~PageManager();

    void add(const std::string& name, std::unique_ptr<Page> page);
    void scrollto(const std::string& name);

private:
    std::list<std::pair<std::string, std::unique_ptr<Page>>> pages {};
};

#endif // MATH_PAGEMANAGER_H