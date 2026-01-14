#pragma once
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"

namespace CardGame {

    class CardView : public cocos2d::Sprite {
    public:
        // 创建视图的入口
        static CardView* createWithModel(CardModel* model);

        bool initWithModel(CardModel* model);

        // 获取持有的数据模型
        CardModel* getModel() const { return _model; }

    private:
        CardModel* _model;

        // --- 辅助工具函数，用于转换文件名 ---
        // 根据花色判断颜色 ("red" 或 "black")
        std::string _getColorName(int suit);
        // 根据数字获取文件名后缀 (1->"A", 11->"J"...)
        std::string _getFaceName(int face);
        // 根据花色枚举获取文件名 ("spade"...)
        std::string _getSuitName(int suit);
    };
}

#endif // __CARD_VIEW_H__