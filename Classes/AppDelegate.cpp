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
    Director* pDirector =Director::sharedDirector();
    pDirector->setDisplayStats(false);
    pDirector->setOpenGLView(EGLView::sharedOpenGLView());
    pDirector->runWithScene(GameScene::scene());

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    Director::sharedDirector()->stopAnimation();
    Director::sharedDirector()->pause();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::sharedDirector()->resume();
    Director::sharedDirector()->startAnimation();
    
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
