#ifndef MATH_QUESTIONSTRING_H
#define MATH_QUESTIONSTRING_H

#include <string>
#include "cocos2d.h"

class QuestionString : public cocos2d::NodeRGBA
{
public:
    QuestionString()
    : lastPos(0)
    , baseSize(0)
    , scaleHeight(0)
    , maxHeight(0)
    {};

    static QuestionString* create(void) = delete;
    static QuestionString* create(const int baseSize);
    bool init(const int baseSize);

    bool isVisible() const;
    void setOpacity(GLubyte opacity) override;
    void setColor(const cocos2d::Color3B& color) override;

    void setText(const std::string& text);
    void setHeight(const float height);

private:
    float lastPos;
    int baseSize;
    float scaleHeight;
    float maxHeight;

    cocos2d::LabelTTF* addSubLabel(const std::string& text, const int fontSize);
    bool replaceSqrt(const std::string& text);
    bool replaceFraction(const std::string& text);
};

#endif // MATH_QUESTIONSTRING_H
