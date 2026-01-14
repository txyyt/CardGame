#pragma once
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "views/GameView.h"
#include "models/CardModel.h"
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

    private:
        // 持有 View 的指针
        GameView* _gameView;

        // 持有所有卡牌数据的列表 (防止内存释放)
        cocos2d::Vector<CardModel*> _cards;
    };
}

#endif // __GAME_CONTROLLER_H__