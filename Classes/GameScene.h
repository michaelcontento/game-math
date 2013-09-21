#ifndef MATH_GAMESCENE_H
#define MATH_GAMESCENE_H

#include "cocos2d.h"
#include <string>

class PageManager;

class GameScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    static cocos2d::Scene* scene();

private:
    PageManager* pageManager = nullptr;
    static std::string searchPath;

    void addCategoryPages(PageManager& pageManager) const;
    static void threadInit();
    static void initAds();
    static void initPayment();
    static void initGameCenter();
    static void initSoundAndMusic();
    static void initAssetsSearchpath();
    static void updateAssets();
    void initLocalization();
    void initAssets();
    void profile();
    void initPages();
};

#endif // MATH_GAMESCENE_H
