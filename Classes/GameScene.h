#ifndef MATH_GAMESCENE_H
#define MATH_GAMESCENE_H

#include "cocos2d.h"
#include "PageManager.h"

class GameScene : public cocos2d::Layer
{
public:
    virtual bool init() override;
    static cocos2d::Scene* scene();
    CREATE_FUNC(GameScene);

private:
    PageManager* pageManager;
};

#endif // MATH_GAMESCENE_H
