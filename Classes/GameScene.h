#ifndef MATH_GAMESCENE_H
#define MATH_GAMESCENE_H

#include "cocos2d.h"

class GameScene : public cocos2d::CCLayer
{
public:
    virtual bool init() override;
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);
};

#endif // MATH_GAMESCENE_H
