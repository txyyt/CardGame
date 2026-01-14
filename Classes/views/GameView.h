#pragma once
#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"

namespace CardGame {

    class GameView : public cocos2d::Layer {
    public:
        // 标准的 Cocos CREATE_FUNC 宏，自动生成 create() 方法
        CREATE_FUNC(GameView);

        virtual bool init() override;
    };
}

#endif // __GAME_VIEW_H__