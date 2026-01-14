#include "GameView.h"

using namespace cocos2d;

namespace CardGame {

    bool GameView::init() {
        if (!Layer::init()) {
            return false;
        }

        // 加载背景(绿色，模拟牌桌)
        auto background = LayerColor::create(Color4B(50, 160, 50, 255));
        this->addChild(background, -1); // z-order -1 保证在最底层

        return true;
    }
}