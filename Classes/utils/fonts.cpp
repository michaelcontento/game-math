#include "fonts.h"

#include "config.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string& text, const unsigned short size)
{
    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-Light",
        size * config::getScaleFactor()
    );
}

cocos2d::LabelTTF* createLight(const std::string& text, const unsigned short size)
{
    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-UltraLight",
        size * config::getScaleFactor()
    );
}

cocos2d::Sprite* createStar(const cocos2d::Color3B& color)
{
    auto star = cocos2d::Sprite::create("star.png");
    star->setColor(color);

    // FIX#5
    star->setScale(config::getScaleFactor());
    
    return star;
}

void fillStarContainer(cocos2d::Node& cocosContainer, std::deque<cocos2d::Sprite*>& cppContainer, const int amount, const cocos2d::Color3B& color)
{
    auto padding = 3 * config::getScaleFactor();
    for (int i = 1; i <= amount; ++i) {
        auto star = fonts::createStar(color);
        cocosContainer.addChild(star);
        cppContainer.push_back(star);

        auto size = star->getContentSize() * star->getScale();
        star->setPositionX(size.width * i + padding * fmax(0, i - 2));

        cocosContainer.setContentSize({star->getPositionX() + size.width, size.height});
    }
}

} // namespace fonts