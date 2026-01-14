#pragma once
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "views/GameView.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include "managers/UndoManager.h" 
#include <vector>

namespace CardGame {

    // 继承 Ref 进行内存管理
    class GameController : public cocos2d::Ref {
    public:
        // 创建并进入游戏场景的入口方法
        static cocos2d::Scene* createGameScene(int levelId);

        static GameController* create();
        bool init();

        // 开始一局游戏
        void startGame(int levelId, GameView* view);

        // 处理卡牌点击
        void onCardClicked(CardView* clickedView);

        // 处理点击备用牌堆
        void onDrawPileClicked();

        // 处理回退按钮点击
        void onUndoClicked();

    private:
        // 持有 View 的指针
        GameView* _gameView;

        // 持有所有卡牌数据的列表 (防止内存释放)
        cocos2d::Vector<CardModel*> _cards;

        // 记录当前的底牌
        // 指向当前底牌堆最上面那张牌
        CardModel* _topStackCard;

        // 备用牌堆数据
        // 存放还没翻出来的牌
        cocos2d::Vector<CardModel*> _drawPileModels;

        // 备用牌堆的显示对象
        cocos2d::Node* _drawPileNode;

        // 辅助函数：根据剩余张数，刷新备用牌堆的样子
        void _updateDrawPileVisual();

        // UndoManager 成员变量
        UndoManager* _undoManager;

        // 辅助函数：根据数据模型找到对应的 View (回退动画用)
        CardView* _findViewByModel(CardModel* model);
    };
}

#endif // __GAME_CONTROLLER_H__