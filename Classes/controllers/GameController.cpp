#include "GameController.h"
#include "configs/LevelConfigLoader.h"
#include "views/CardView.h"

using namespace cocos2d;

namespace CardGame {

    cocos2d::Scene* GameController::createGameScene(int levelId) {
        // 1. 创建物理场景
        auto scene = Scene::create();

        // 2. 创建 View (显示层)
        auto view = GameView::create();
        scene->addChild(view);

        // 3. 创建 Controller (逻辑层)
        auto controller = GameController::create();

        // 4. Controller 开始接管，加载数据
        controller->startGame(levelId, view);

        // 为了保持 Controller 存活，通常将其挂载到 Scene 上 (作为 UserObject 或 Component)
        // 这里简单起见，利用 setUserObject 绑定生命周期
        scene->setUserObject(controller);

        return scene;
    }

    GameController* GameController::create() {
        GameController* ret = new (std::nothrow) GameController();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool GameController::init() {
        _gameView = nullptr;
        return true;
    }

    void GameController::startGame(int levelId, GameView* view) {
        _gameView = view;
        _cards.clear();

        // 1. 读取关卡配置 (JSON)
        LevelConfigData data = LevelConfigLoader::loadLevelConfig(levelId);

        // 2. 生成主牌区的牌 (Playfield)
        for (const auto& cardData : data.playfieldCards) {
            // A. 创建数据模型 Model
            auto cardModel = CardModel::create(cardData.cardFace, cardData.cardSuit);
            _cards.pushBack(cardModel); // 保存引用

            // B. 创建视图 View
            auto cardView = CardView::createWithModel(cardModel);

            // C. 设置位置 (读取 JSON 里的 x, y)
            cardView->setPosition(cardData.position);

            // D. 添加到界面
            _gameView->addChild(cardView);
        }

        // 3. 生成底部牌堆的牌 (Stack)
        for (const auto& cardData : data.stackCards) {
            auto cardModel = CardModel::create(cardData.cardFace, cardData.cardSuit);
            _cards.pushBack(cardModel);

            auto cardView = CardView::createWithModel(cardModel);
            // 底部牌堆通常在下方，如果 JSON 里是 0,0，我们先强制给个位置看看效果
            if (cardData.position.equals(Vec2::ZERO)) {
                cardView->setPosition(Vec2(540, 300)); // 屏幕中间下方
            }
            else {
                cardView->setPosition(cardData.position);
            }
            _gameView->addChild(cardView);
        }
    }
}