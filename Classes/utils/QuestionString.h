#ifndef MATH_QUESTIONSTRING_H
#define MATH_QUESTIONSTRING_H

#include <string>
#include "cocos2d.h"

class QuestionString : public cocos2d::NodeRGBA
{
public:
    bool bold = false;
    
    static QuestionString* create(void) = delete;
    static QuestionString* create(const int baseSize);
    bool init(const int baseSize);

    bool isVisible() const;
    void setOpacity(GLubyte opacity) override;
    void setColor(const cocos2d::Color3B& color) override;

    void setText(const std::string& text);

private:
    float lastPos = 0;
    int baseSize = 0;

    cocos2d::LabelTTF* addSubLabel(const std::string& text, const int fontSize);
    bool replaceSqrt(const std::string& text);
    bool replaceFraction(const std::string& text);
};

#endif // MATH_QUESTIONSTRING_H