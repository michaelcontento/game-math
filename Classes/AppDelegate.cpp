#include "AppDelegate.h"

#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "utils/user.h"

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
    pDirector->setDisplayStats(false    );

    pDirector->setOpenGLView(EGLView::getInstance());

    // set white clear color and clear once to prevent a short flicker
    // between the ios loading image and the first cocos2dx loop iteration
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    EGLView::getInstance()->swapBuffers();

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

    if (user::hasMusicEnabled()) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    SimpleAudioEngine::getInstance()->resumeAllEffects();
}
