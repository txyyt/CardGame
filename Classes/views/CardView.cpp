#include "CardView.h"

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
        _model->retain(); // 对应 release 在析构函数中（这里简单起见省略析构，实际项目要写）

        // 1. 加载底板 (Background)
        // 路径基于你的截图：Resources/res/card_general.png
        // 注意：Cocos 默认从 Resources 根目录开始找，所以要带上 res/ 前缀
        if (!Sprite::initWithFile("res/card_general.png")) {
            CCLOG("Error: Failed to load res/card_general.png");
            return false;
        }

        Size size = this->getContentSize(); // 获取底板大小，用于定位子元素

        // 准备数据字符串
        int suit = model->getSuit();
        int face = model->getFace();
        std::string color = _getColorName(suit); // "red" or "black"
        std::string faceStr = _getFaceName(face); // "A", "2"...
        std::string suitStr = _getSuitName(suit); // "heart"...

        // 2. 添加中间的大数字 (Big Number)
        // 你的文件命名格式推测为: big_black_A.png
        std::string bigNumPath = StringUtils::format("res/number/big_%s_%s.png", color.c_str(), faceStr.c_str());
        auto bigNum = Sprite::create(bigNumPath);
        if (bigNum) {
            // 居中显示
            bigNum->setPosition(Vec2(size.width / 2, size.height / 2));
            this->addChild(bigNum);
        }
        else {
            CCLOG("Cannot find file: %s", bigNumPath.c_str());
        }

        // 3. 添加左上角的小数字 (Small Number)
        // 你的文件命名格式推测为: small_black_A.png
        std::string smallNumPath = StringUtils::format("res/number/small_%s_%s.png", color.c_str(), faceStr.c_str());
        auto smallNum = Sprite::create(smallNumPath);
        float smallNumHeight = 0;

        if (smallNum) {
            // 设置锚点为【顶部中间】，方便定位
            smallNum->setAnchorPoint(Vec2(0.5, 1.0));
            // 放在左上角：X=30, Y=高度-20 (留点边距)
            smallNum->setPosition(Vec2(30, size.height - 20));
            this->addChild(smallNum);
            smallNumHeight = smallNum->getContentSize().height;
        }

        // 4. 添加左上角的花色 (Suit Icon)
        // 你的文件命名格式推测为: heart.png, spade.png...
        std::string suitPath = StringUtils::format("res/suits/%s.png", suitStr.c_str());
        auto suitIcon = Sprite::create(suitPath);
        if (suitIcon) {
            suitIcon->setAnchorPoint(Vec2(0.5, 1.0));
            // 放在小数字的正下方 5个像素处
            suitIcon->setPosition(Vec2(30, size.height - 20 - smallNumHeight - 5));
            // 如果花色图太大，可能需要缩放，这里先缩小一半看看效果
            suitIcon->setScale(0.5f);
            this->addChild(suitIcon);
        }

        return true;
    }

    // 辅助函数实现
    std::string CardView::_getColorName(int suit) {
        // 方块(1) 和 红桃(2) 是红色
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