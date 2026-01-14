#pragma once
#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "configs/GameConstants.h"

namespace CardGame {

    // 继承 Ref 是为了使用 Cocos 的引用计数内存管理
    class CardModel : public cocos2d::Ref {
    public:
        // 静态创建方法
        static CardModel* create(int face, int suit);

        // 初始化
        bool init(int face, int suit);

        // 获取数据
        int getFace() const { return _face; }
        int getSuit() const { return _suit; }

        // 状态管理
        bool isFaceUp() const { return _isFaceUp; }
        void setFaceUp(bool isFaceUp) { _isFaceUp = isFaceUp; }

    private:
        int _face;      // 点数
        int _suit;      // 花色
        bool _isFaceUp; // 是否正面朝上
    };
}

#endif // __CARD_MODEL_H__