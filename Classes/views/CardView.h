#pragma once
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional> 

namespace CardGame {

    class CardView : public cocos2d::Sprite {
    public:
        static CardView* createWithModel(CardModel* model);
        bool initWithModel(CardModel* model);
        CardModel* getModel() const { return _model; }

        // 点击回调函数
        // 外部（Controller）通过调用这个方法，告诉 View 被点时该通知谁
        void setClickCallback(std::function<void(CardView*)> callback);

    private:
        CardModel* _model;

        // --- 新增：保存回调函数 ---
        std::function<void(CardView*)> _clickCallback;

        // 私有辅助函数
        std::string _getColorName(int suit);
        std::string _getFaceName(int face);
        std::string _getSuitName(int suit);
    };
}

#endif // __CARD_VIEW_H__