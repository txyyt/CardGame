#include "CardModel.h"

namespace CardGame {

    CardModel* CardModel::create(int face, int suit) {
        CardModel* model = new (std::nothrow) CardModel();
        if (model && model->init(face, suit)) {
            model->autorelease();
            return model;
        }
        CC_SAFE_DELETE(model);
        return nullptr;
    }

    bool CardModel::init(int face, int suit) {
        _face = face;
        _suit = suit;
        _isFaceUp = true; // 默认正面朝上
        return true;
    }
}