#ifndef MATH_GAMESCENE_H
#define MATH_GAMESCENE_H

#include "cocos2d.h"

class PageManager;

class GameScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    static cocos2d::Scene* scene();

private:
    PageManager* pageManager = nullptr;
};

#endif // MATH_GAMESCENE_H
