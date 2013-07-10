#include "GameScene.h"

#include "pages/MainPage.h"
#include "pages/SettingsPage.h"
#include "pages/CategoryPage.h"
#include "PageManager.h"
#include "puzzle/Generator.h"

using namespace cocos2d;

Scene* GameScene::scene()
{
    Scene* scene = Scene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }

    // number
    // operator
    // hidden
    puzzle::Generator g1 {
        "{number} {operator} {number}",
        "{number}",
        {puzzle::Operator::PLUS},
        {puzzle::NumberRange::SMALL}
    };
    auto q1 = g1.generate();
    CCLog("%s [%s] %s %s", q1.question.c_str(), q1.rightAnswer.c_str(), q1.wrongAnswer1.c_str(), q1.wrongAnswer2.c_str());
    /*
    Generator g2 {"{number} {operator} {hidden}", "{hidden}"};
    Generator g3 {"{answer}", "{number} {operator} {number}"};
    Generator g4 {"numberest?", "{number} {operator} {number}"}; // ?
    Generator g5 {"{number} {operator} {number} {operator} {number}", "{number}"};
    Generator g6 {"{number} {operator} {number}", "{number} {operator} {number}"}; // ?
    Generator g7 {"Largest?", "{number} {operator} {number}"}; // ?
    GeneratorMixer g8 {g1, g2, g3, g4, g5, g6, g7};
    */

    pageManager = PageManager::create();
    addChild(pageManager);

    pageManager->add("settings", SettingsPage::create());
    pageManager->add("main", MainPage::create());
    pageManager->add("category-01", CategoryPage::create());
    pageManager->scrollTo("main", 0);

    return true;
}