#include "CardView.h"
#include "configs/GameConstants.h" 

using namespace cocos2d;

namespace CardGame { 

    CardView* CardView::createWithModel(CardModel* model) {
        CardView* view = new (std::nothrow) CardView();
        if (view && view->initWithModel(model)) {
            view->autorelease();
            return view;
        }
        CC_SAFE_DELETE(view);
        return nullptr;
    }

    bool CardView::initWithModel(CardModel* model) {
        _model = model;
        _model->retain();

        // 1. 加载底板
        if (!Sprite::initWithFile("res/card_general.png")) {
            CCLOG("Error: Failed to load res/card_general.png");
            return false;
        }

        Size size = this->getContentSize();

        // 准备数据
        int suit = model->getSuit();
        int face = model->getFace();
        std::string color = _getColorName(suit);
        std::string faceStr = _getFaceName(face);
        std::string suitStr = _getSuitName(suit);

        // 2. 中间大数字
        std::string bigNumPath = StringUtils::format("res/number/big_%s_%s.png", color.c_str(), faceStr.c_str());
        auto bigNum = Sprite::create(bigNumPath);
        if (bigNum) {
            bigNum->setPosition(Vec2(size.width / 2, size.height / 2));
            this->addChild(bigNum);
        }

        // 3. 左上角小数字
        std::string smallNumPath = StringUtils::format("res/number/small_%s_%s.png", color.c_str(), faceStr.c_str());
        auto smallNum = Sprite::create(smallNumPath);
        float smallNumHeight = 0;
        if (smallNum) {
            smallNum->setAnchorPoint(Vec2(0.5, 1.0));
            smallNum->setPosition(Vec2(30, size.height - 20));
            this->addChild(smallNum);
            smallNumHeight = smallNum->getContentSize().height;
        }

        // 4. 左上角花色
        std::string suitPath = StringUtils::format("res/suits/%s.png", suitStr.c_str());
        auto suitIcon = Sprite::create(suitPath);
        if (suitIcon) {
            suitIcon->setAnchorPoint(Vec2(0.5, 1.0));
            suitIcon->setPosition(Vec2(30, size.height - 20 - smallNumHeight - 5));
            suitIcon->setScale(0.8f);
            this->addChild(suitIcon);
        }

        // 触摸事件监听 
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);

        listener->onTouchBegan = [this](Touch* touch, Event* event) {
            Vec2 p = this->convertTouchToNodeSpace(touch);
            Size s = this->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            if (rect.containsPoint(p)) {
                return true;
            }
            return false;
            };

        listener->onTouchEnded = [this](Touch* touch, Event* event) {
            if (_clickCallback) {
                _clickCallback(this);
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    } 

    // 实现设置回调的方法
    void CardView::setClickCallback(std::function<void(CardView*)> callback) {
        _clickCallback = callback;
    }

    // 辅助函数 
    std::string CardView::_getColorName(int suit) {
        if (suit == CST_DIAMONDS || suit == CST_HEARTS) return "red";
        return "black";
    }

    std::string CardView::_getFaceName(int face) {
        switch (face) {
        case 1: return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return StringUtils::format("%d", face);
        }
    }

    std::string CardView::_getSuitName(int suit) {
        switch (suit) {
        case CST_CLUBS: return "club";
        case CST_DIAMONDS: return "diamond";
        case CST_HEARTS: return "heart";
        case CST_SPADES: return "spade";
        default: return "spade";
        }
    }

} 