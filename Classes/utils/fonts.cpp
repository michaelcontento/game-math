#include "fonts.h"
#include "config.h"

namespace fonts {

cocos2d::LabelTTF* createNormal(const std::string text, const int size)
{
    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-Light",
        size * config::getScaleFactor()
    );
}

cocos2d::LabelTTF* createLight(const std::string text, const int size)
{
    return cocos2d::LabelTTF::create(
        text.c_str(),
        "HelveticaNeue-UltraLight",
        size * config::getScaleFactor()
    );
}

} // namespace fonts