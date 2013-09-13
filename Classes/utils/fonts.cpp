#include "fonts.h"

#include "config.h"
#include "user.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string& text, const unsigned short size, cocos2d::TextHAlignment hAlignment, cocos2d::TextVAlignment vAlignment, const unsigned short maxSize)
{
    auto fontSize = size * config::getScaleFactor();
    if (maxSize > 0) {
        fontSize = MIN(maxSize, fontSize);
    }

    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-Medium",
        fontSize,
        cocos2d::Size::ZERO,
        hAlignment,
        vAlignment
    );
}

cocos2d::LabelTTF* createLight(const std::string& text, const unsigned short size, cocos2d::TextHAlignment hAlignment, cocos2d::TextVAlignment vAlignment, const unsigned short maxSize)
{
    auto fontSize = size * config::getScaleFactor();
    if (maxSize > 0) {
        fontSize = MIN(maxSize, fontSize);
    }

    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-Light",
        fontSize,
        cocos2d::Size::ZERO,
        hAlignment,
        vAlignment
    );
}

cocos2d::Sprite* createStar(const cocos2d::Color3B& color, const bool filled, const bool magic)
{
    auto star = cocos2d::Sprite::createWithSpriteFrameName(filled ? "star-full.png" : "star-empty.png");
    star->setColor(color);
    const auto factor = magic ? config::getScaleFactorHeightMagic() : config::getScaleFactorHeight();
    star->setScale(0.5 * config::getScaleFactor() * factor);
    return star;
}

void fillStarContainer(cocos2d::Node& cocosContainer, std::deque<cocos2d::Sprite*>& cppContainer, const int amount, const cocos2d::Color3B& color, const bool filled, const bool magic)
{
    auto padding = 3 * config::getScaleFactor();
    for (int i = 1; i <= amount; ++i) {
        auto star = fonts::createStar(color, filled, magic);
        cocosContainer.addChild(star);
        cppContainer.push_back(star);

        auto size = star->getContentSize() * star->getScale();
        star->setPositionX(size.width * (i - 1) + padding * fmax(0, i - 1));
        star->setAnchorPoint(cocos2d::Point::ZERO);

        cocosContainer.setContentSize({star->getPositionX() + size.width, size.height});
    }
}

} // namespace fonts