#include "QuestionString.h"

#include <avalon/utils/platform.h>
#include "fonts.h"
#include "config.h"

using namespace cocos2d;

QuestionString* QuestionString::create(const int baseSize)
{
    QuestionString* pRet = new QuestionString();
    if (pRet && pRet->init(baseSize)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool QuestionString::init(const int baseSize)
{
    if (!NodeRGBA::init()) {
        return false;
    }

    this->baseSize = baseSize;
    setAnchorPoint({0.5, 0});

    return true;
}

cocos2d::LabelTTF* QuestionString::addSubLabel(const std::string& text, const int fontSize)
{
    LabelTTF* label = fonts::createLight(text, fontSize, TextHAlignment::CENTER, TextVAlignment::CENTER);
    addChild(label);

    label->setColor(getColor());
    label->setAnchorPoint({0, 0.5});
    label->setPositionX(lastPos);

    const static auto padding = (1 * config::getScaleFactor());
    lastPos += label->getContentSize().width + padding;

    maxHeight = std::max(maxHeight, label->getContentSize().height);

    return label;
};

bool QuestionString::replaceSqrt(const std::string& text)
{
    const static auto lineLeftOffset = 7 * config::getScaleFactor();
    auto work = text;
    auto pos = work.find("SQR#");
    if (pos == std::string::npos) {
        return false;
    }

#if AVALON_PLATFORM_IS_TIZEN
    const static auto signPadding = (12 * config::getScaleFactor());
#else
    const static auto signPadding = (3.5 * config::getScaleFactor());
#endif

    while (pos != std::string::npos) {
        // text before "SQR#"
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), baseSize);
        }
        work = work.substr(pos + 4);

        // square root sign
#if AVALON_PLATFORM_IS_TIZEN
        auto sign = addSubLabel("√", baseSize * 0.975);
#else
        auto sign = addSubLabel("√", baseSize * 0.875);
#endif
        sign->setPositionY(sign->getPositionY() + signPadding);

        // add the text to the next #
        const auto oldLastPos = lastPos;
        pos = work.find("#");
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), baseSize);
        }
        work = work.substr(pos + 1);

        // extend the line of the square root sign
        const auto restPos = lastPos;
        const auto lineLenght = lastPos - oldLastPos + lineLeftOffset;

        std::string lineStr = "_";
        auto line = addSubLabel(lineStr, baseSize);
        lastPos = restPos;

        while (line->getContentSize().width <= lineLenght) {
            lineStr += "_";
            line->setString(lineStr.c_str());
        }
        line->setScaleX(lineLenght / line->getContentSize().width);

        line->setPositionX(oldLastPos - lineLeftOffset);
#if AVALON_PLATFORM_IS_TIZEN
        line->setPositionY(static_cast<int>(baseSize * 0.962) * config::getScaleFactor());
#else
        line->setPositionY(static_cast<int>(baseSize * 0.862) * config::getScaleFactor());
#endif

        // prepare for the next loop iteration
        pos = work.find("SQR#");
    }

    if (work.length() > 0) {
        addSubLabel(work, baseSize);
    }
    return true;
}

bool QuestionString::replaceFraction(const std::string& text)
{
    const auto verticalOffset = static_cast<int>(baseSize * 0.328) * config::getScaleFactor();
    static const auto lineExtra = 2 * config::getScaleFactor();
    auto work = text;
    auto pos = work.find("FRC#");
    if (pos == std::string::npos) {
        return false;
    }

#if AVALON_PLATFORM_IS_TIZEN
    static float tizenSpacing = 20 * config::getScaleFactor();
#endif

    while (pos != std::string::npos) {
        // text before "FRC#"
        if (pos > 0) {
            addSubLabel(work.substr(0, pos), baseSize);
        }
        work = work.substr(pos + 4);

#if AVALON_PLATFORM_IS_TIZEN
        // front spacing
        if (lastPos > 0) {
            lastPos += tizenSpacing;
        }
#endif
        const auto oldLastPos = lastPos;

        // get the upper text
        pos = work.find(";");
        auto upper = addSubLabel(work.substr(0, pos), baseSize * 0.632);
        lastPos = oldLastPos;
        upper->setPositionY(upper->getPositionY() + verticalOffset);
        work = work.substr(pos + 1);
        maxHeight = std::max(maxHeight, verticalOffset + upper->getContentSize().height);

        // get the lower text
        pos = work.find("#");
        auto lower = addSubLabel(work.substr(0, pos), baseSize * 0.632);
        lastPos = oldLastPos;
        lower->setPositionY(lower->getPositionY() - verticalOffset);
        work = work.substr(pos + 1);

        // align both upper and lower numbers
        const auto upperWidth = upper->getContentSize().width;
        const auto lowerWidth = lower->getContentSize().width;
        if (upperWidth > lowerWidth) {
            lower->setPositionX(lower->getPositionX() + (upperWidth - lowerWidth) / 2.0);
        } else if (lowerWidth > upperWidth) {
            upper->setPositionX(upper->getPositionX() + (lowerWidth - upperWidth) / 2.0);
        }

        // add the middle line
        const auto lineLenght = std::max(
            lower->getContentSize().width,
            upper->getContentSize().width
        ) + (lineExtra * 2);

        std::string lineStr = "_";
        auto line = addSubLabel(lineStr, static_cast<int>(baseSize * 0.6666666667));
        lastPos = oldLastPos;

        while (line->getContentSize().width <= lineLenght) {
            lineStr += "_";
            line->setString(lineStr.c_str());
        }
        line->setScaleX(lineLenght / line->getContentSize().width);

        line->setPositionX(lastPos - lineExtra);
        line->setPositionY(static_cast<int>(baseSize * 0.292) * config::getScaleFactor());

        // advance for the following characters
        lastPos += lineLenght - (lineExtra * 2);

#if AVALON_PLATFORM_IS_TIZEN
        // back spacing
        lastPos += tizenSpacing;
#endif

        // prepare for the next loop iteration
        pos = work.find("FRC#");
    }

    if (work.length() > 0) {
        addSubLabel(work, baseSize);
    }
    return true;
}

bool QuestionString::isVisible() const
{
    Object* it = nullptr;
    CCARRAY_FOREACH(getChildren(), it) {
        const auto child = dynamic_cast<RGBAProtocol*>(it);
        if (!child) {
            continue;
        }

        return child->getOpacity() == 255;
    }
    return false;
}

void QuestionString::setOpacity(GLubyte opacity)
{
    Object* it = nullptr;
    CCARRAY_FOREACH(getChildren(), it) {
        const auto child = dynamic_cast<RGBAProtocol*>(it);
        if (!child) {
            continue;
        }

        child->setOpacity(opacity);
    }
}

void QuestionString::setText(const std::string& text)
{
    removeAllChildren();
    lastPos = 0;
    maxHeight = 0;

    if (text.find("#") == std::string::npos) {
        addSubLabel(text, baseSize);
    } else {
        if (!replaceSqrt(text)) {
            if (!replaceFraction(text)) {
                addSubLabel(text, baseSize);
            }
        }
    }

    setContentSize(Size(lastPos, 0));

    const static auto maxWidth = config::getFrameSize().width * 0.9;
    if (lastPos > maxWidth) {
        setScale(maxWidth / lastPos);
    } else {
        setScale(1);
    }

    if (scaleHeight > 0) {
        const float heightScale = scaleHeight / maxHeight;
        if (heightScale < getScale()) {
            setScale(heightScale);
        }
    }
}

void QuestionString::setColor(const cocos2d::Color3B& color)
{
    NodeRGBA::setColor(color);

    Object* it = nullptr;
    CCARRAY_FOREACH(getChildren(), it) {
        const auto child = dynamic_cast<RGBAProtocol*>(it);
        if (!child) {
            continue;
        }

        child->setColor(color);
    }
}

void QuestionString::setHeight(const float height)
{
    scaleHeight = height;
}
