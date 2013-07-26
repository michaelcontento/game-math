#include "helper.h"

#include "config.h"

using namespace cocos2d;

namespace helper {

bool isNodeInViewRect(const cocos2d::Node& node)
{
    static const Rect viewRect {0, 0, config::getFrameSize().width, config::getFrameSize().height};
    return viewRect.containsPoint(node.convertToWorldSpace(node.getPosition()));
}

} // namespace helper
