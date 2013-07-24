#include "AppDelegate.h"

#include "SimpleAudioEngine.h"
#include "GameScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    Director* pDirector = Director::getInstance();
    pDirector->setDisplayStats(false);

    pDirector->setOpenGLView(EGLView::getInstance());
    glClearColor(1.0, 1.0, 1.0, 1.0);

    pDirector->runWithScene(GameScene::scene());

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
    
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
}
