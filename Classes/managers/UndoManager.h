#pragma once
#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "models/UndoModel.h" 
#include "models/CardModel.h"
#include <vector>

namespace CardGame {

    class UndoManager : public cocos2d::Ref {
    public:
        static UndoManager* create();
        bool init();

        // 1. 记录一步操作 (入栈)
        // 当你消除一张牌时，调用这个
        void pushOperation(CardModel* movedCard, cocos2d::Vec2 originalPos, int zOrder, CardModel* prevTopCard);

        // 2. 获取并移除最后一步操作 (出栈)
        // 当你点 UNDO 按钮时，调用这个
        UndoModel* popLastOperation();

        // 3. 清空所有记录 (比如新游戏开始时)
        void clear();

        // 4. 判断按钮是否可点
        bool canUndo() const;

    private:
        // 使用 Vector 自动管理 Ref 对象的内存引用计数，防止野指针
        cocos2d::Vector<UndoModel*> _historyStack;
    };
}

#endif // __UNDO_MANAGER_H__