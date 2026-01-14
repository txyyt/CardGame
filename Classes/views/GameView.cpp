#include "GameView.h"

using namespace cocos2d;

namespace CardGame {

    bool GameView::init() {
        if (!Layer::init()) {
            return false;
        }

        Size visibleSize = Director::getInstance()->getVisibleSize();

        // 设定分界线高度 
        float splitHeight = 900.0f;

        // 1. 上半部分：主牌区 (浅绿色)
        auto topLayer = LayerColor::create(Color4B(50, 160, 50, 255), visibleSize.width, visibleSize.height - splitHeight);
        topLayer->setPosition(0, splitHeight); // 从分界线开始往上画
        this->addChild(topLayer, -1);

        // 2. 下半部分：底牌/操作区 
        auto bottomLayer = LayerColor::create(Color4B(20, 80, 30, 255), visibleSize.width, splitHeight);
        bottomLayer->setPosition(0, 0); // 从底部开始画
        this->addChild(bottomLayer, -1);

        // 3. 加一条金色的分界线
        auto line = DrawNode::create();
        line->drawLine(Vec2(0, splitHeight), Vec2(visibleSize.width, splitHeight), Color4F(1.0f, 0.84f, 0.0f, 1.0f)); // 金色
        this->addChild(line, -1);

        return true;
    }
}