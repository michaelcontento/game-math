#ifndef MATH_ANSWERBUTTON_H
#define MATH_ANSWERBUTTON_H

#include "cocos2d.h"

class AnswerButton : public cocos2d::Node
{
public:
    static AnswerButton* create(void) = delete;
    static AnswerButton* create(const cocos2d::Color3B& color);
    bool init(const cocos2d::Color3B& color);

    void onTouch(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    void configureSize();
    void addBackground(const cocos2d::Color3B& color);
    bool hasBeenTouched(cocos2d::Touch* touch, cocos2d::Event* event);
    void addLabel();
};

#endif // MATH_ANSWERBUTTON_H