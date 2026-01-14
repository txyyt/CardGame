#pragma once
#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"

namespace CardGame {

    // 这是一个纯数据类，用来记录“一步操作”的所有信息
    class UndoModel : public cocos2d::Ref {
    public:
        // 标准的创建方法
        static UndoModel* create() {
            UndoModel* ret = new (std::nothrow) UndoModel();
            if (ret) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        // 1. 被移走的牌
        CardModel* movedCard;

        // 2. 原来在屏幕的什么位置(x, y)
        cocos2d::Vec2 originalPos;

        // 3. 原来的层级(防止回退后盖住别的牌)
        int originalZOrder;

        // 4. 底牌堆顶是哪张牌
        CardModel* previousTopCard;

        // 构造函数初始化
        UndoModel() : movedCard(nullptr), previousTopCard(nullptr), originalZOrder(0) {}
    };
}

#endif // __UNDO_MODEL_H__