#pragma once
#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

namespace CardGame {

    // 单个卡牌数据
    struct CardConfigData {
        int cardFace; // 牌面数值 (1-13)
        int cardSuit; // 花色 (0-3)
        cocos2d::Vec2 position; // 坐标 x, y

        // 构造函数：初始化默认值，防止乱码
        CardConfigData() : cardFace(0), cardSuit(0), position(cocos2d::Vec2::ZERO) {}
    };

    // 数据结构
    // 包含 "Playfield" 数组和 "Stack" 数组
    struct LevelConfigData {
        // 主牌区的所有卡牌配置
        std::vector<CardConfigData> playfieldCards;

        // 底部牌堆的所有卡牌配置
        std::vector<CardConfigData> stackCards;
    };

    /*
     * LevelConfig 类
     * 作为静态配置数据的载体
     */
    class LevelConfig {
    public:
        
    };
}

#endif // __LEVEL_CONFIG_H__·