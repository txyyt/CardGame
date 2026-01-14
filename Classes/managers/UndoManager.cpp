#include "UndoManager.h"

namespace CardGame {

    UndoManager* UndoManager::create() {
        UndoManager* ret = new (std::nothrow) UndoManager();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool UndoManager::init() {
        return true;
    }

    void UndoManager::pushOperation(CardModel* movedCard, cocos2d::Vec2 originalPos, int zOrder, CardModel* prevTopCard) {
        auto op = UndoModel::create();

        // 记录数据
        op->movedCard = movedCard;
        op->originalPos = originalPos;
        op->originalZOrder = zOrder;
        op->previousTopCard = prevTopCard;

        // 存入栈顶
        _historyStack.pushBack(op);
    }

    UndoModel* UndoManager::popLastOperation() {
        if (_historyStack.empty()) {
            return nullptr;
        }

        // 取出最后一个
        UndoModel* lastOp = _historyStack.back();

        // 增加引用计数，防止从 Vector 移除时被自动释放销毁
        lastOp->retain();

        _historyStack.popBack();

        // 设置为自动释放，交给调用者（GameController）去管理生命周期
        lastOp->autorelease();

        return lastOp;
    }

    void UndoManager::clear() {
        _historyStack.clear();
    }

    bool UndoManager::canUndo() const {
        return !_historyStack.empty();
    }
}