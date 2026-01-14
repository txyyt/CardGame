#include "GameController.h"
#include "configs/LevelConfigLoader.h"
#include "views/CardView.h"
#include "managers/UndoManager.h" 

using namespace cocos2d;

// 定义位置常量
const Vec2 POS_DRAW_PILE(300, 650);   // 左边：备用牌堆位置
const Vec2 POS_BOTTOM_PILE(780, 650); // 右边：当前底牌位置

namespace CardGame {

    cocos2d::Scene* GameController::createGameScene(int levelId) {
        auto scene = Scene::create();
        auto view = GameView::create();
        scene->addChild(view);

        auto controller = GameController::create();
        controller->startGame(levelId, view);

        // 绑定生命周期
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
        _drawPileNode = nullptr;
        _topStackCard = nullptr;

        // --- 初始化 UndoManager ---
        _undoManager = UndoManager::create();
        _undoManager->retain(); // 必须 retain，防止被自动释放

        return true;
    }

    // 记得在析构函数里释放 (虽然这里没写析构函数，建议加上)
    // GameController::~GameController() { CC_SAFE_RELEASE(_undoManager); }

    void GameController::startGame(int levelId, GameView* view) {
        _gameView = view;
        _cards.clear();
        _drawPileModels.clear();
        _topStackCard = nullptr;
        if (_undoManager) _undoManager->clear();

        // 1. 清理旧的备用牌堆节点
        if (_drawPileNode) {
            _drawPileNode->removeFromParent();
            _drawPileNode = nullptr;
        }

        // 创建新的备用牌堆容器，并固定在左边位置
        _drawPileNode = Node::create();
        _drawPileNode->setPosition(POS_DRAW_PILE);
        _gameView->addChild(_drawPileNode);

        // 加载数据
        LevelConfigData data = LevelConfigLoader::loadLevelConfig(levelId);

        // --- 2. 主牌区 (Playfield) --- 
        float offsetY = 650.0f; // 向上偏移

        for (const auto& cardData : data.playfieldCards) {
            auto cardModel = CardModel::create(cardData.cardFace, cardData.cardSuit);
            _cards.pushBack(cardModel);

            auto cardView = CardView::createWithModel(cardModel);

            // 在这里给原来的坐标加上偏移量
            Vec2 finalPos = cardData.position + Vec2(0, offsetY);
            cardView->setPosition(finalPos);

            _gameView->addChild(cardView);

            cardView->setClickCallback([this](CardView* view) {
                this->onCardClicked(view);
                });
        }

        // --- 3. 底牌区数据处理 (不变) ---
        if (!data.stackCards.empty()) {
            for (size_t i = 0; i < data.stackCards.size() - 1; i++) {
                auto cardData = data.stackCards[i];
                auto cardModel = CardModel::create(cardData.cardFace, cardData.cardSuit);
                _cards.pushBack(cardModel);
                _drawPileModels.pushBack(cardModel);
            }
            // 初始底牌
            auto lastCardData = data.stackCards.back();
            auto bottomModel = CardModel::create(lastCardData.cardFace, lastCardData.cardSuit);
            _cards.pushBack(bottomModel);
            auto bottomView = CardView::createWithModel(bottomModel);
            bottomView->setPosition(POS_BOTTOM_PILE);
            _gameView->addChild(bottomView);
            _topStackCard = bottomModel;
        }

        // --- 4. 刷新备用牌堆视觉效果---
        this->_updateDrawPileVisual();

        // --- 5. 添加备用牌堆点击事件 ---
        // 我们直接把监听器加到 _drawPileNode 上，但是要自己判断点击范围
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* t, Event* e) {
            // 如果备用堆没牌了，就不让点
            if (_drawPileModels.empty()) return false;

            // 简单的距离检测：点中 POS_DRAW_PILE 半径 60 像素范围内就算点中
            if (t->getLocation().distance(POS_DRAW_PILE) < 60) {
                return true;
            }
            return false;
            };
        listener->onTouchEnded = [this](Touch* t, Event* e) {
            this->onDrawPileClicked();
            };
        _gameView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _drawPileNode);

        // --- 6. 添加回退按钮 (UI) ---
        // 创建一个节点作为按钮容器
        auto btnNode = Node::create();
        // 设置位置
        btnNode->setPosition(Vec2(POS_BOTTOM_PILE.x + 100, POS_BOTTOM_PILE.y - 320));
        _gameView->addChild(btnNode, 1000);

        // A. 绘制按钮背景 (圆角矩形)
        auto btnBg = DrawNode::create();
        // 画一个宽160，高60的橙色矩形
        Vec2 rect[] = { Vec2(-80, -30), Vec2(80, -30), Vec2(80, 30), Vec2(-80, 30) };
        btnBg->drawPolygon(rect, 4, Color4F(1.0f, 0.6f, 0.0f, 1.0f), 2, Color4F(1, 1, 1, 1));
        btnNode->addChild(btnBg);

        // B. 添加文字
        auto undoLabel = Label::createWithSystemFont("UNDO", "Arial", 32); // 字号改小一点，精致一点
        undoLabel->setTextColor(Color4B::WHITE);
        undoLabel->enableBold(); // 加粗
        btnNode->addChild(undoLabel);

        // C. 添加触摸事件
        auto undoListener = EventListenerTouchOneByOne::create();
        undoListener->onTouchBegan = [btnNode, btnBg](Touch* t, Event* e) {
            // 把点击坐标转换到按钮节点的局部坐标系
            Vec2 localPos = btnNode->convertTouchToNodeSpace(t);
            // 简单的矩形碰撞检测 (80x30是上面定义的半宽半高)
            if (std::abs(localPos.x) < 80 && std::abs(localPos.y) < 30) {
                // 按下变色反馈 (变暗)
                btnBg->setScale(0.95f); // 稍微缩小
                btnBg->setColor(Color3B(200, 200, 200));
                return true;
            }
            return false;
            };

        undoListener->onTouchEnded = [this, btnBg](Touch* t, Event* e) {
            // 恢复颜色和大小
            btnBg->setScale(1.0f);
            btnBg->setColor(Color3B::WHITE);

            // 执行回退
            this->onUndoClicked();
            };
        _gameView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(undoListener, undoLabel);
    }

    // 翻备用牌逻辑
    void GameController::onDrawPileClicked() {
        if (_drawPileModels.empty()) return;

        // 1. 取数据
        CardModel* newCardModel = _drawPileModels.back();
        _drawPileModels.popBack();

        // 2. 立刻刷新左边的牌堆外观
        this->_updateDrawPileVisual();

        // 3. 创建飞行的 View (起点就是牌堆位置)
        // 稍微加点 ZOrder (1000) 保证飞的时候在最上层
        auto newCardView = CardView::createWithModel(newCardModel);
        newCardView->setPosition(POS_DRAW_PILE);
        newCardView->setScale(0.1f);
        _gameView->addChild(newCardView, 1000);

        // 4. 记录 Undo (你的代码里应该有这部分)
        if (_undoManager) {
            _undoManager->pushOperation(newCardModel, POS_DRAW_PILE, 0, _topStackCard);
        }

        // 5. 动画
        auto spawn = Spawn::create(
            MoveTo::create(0.3f, POS_BOTTOM_PILE),
            ScaleTo::create(0.3f, 1.0f),
            nullptr
        );

        auto callback = CallFunc::create([this, newCardModel, newCardView]() {
            this->_topStackCard = newCardModel;
            newCardView->setLocalZOrder(100);
            });

        newCardView->runAction(Sequence::create(spawn, callback, nullptr));
    }

    // 绘制叠放效果
    void GameController::_updateDrawPileVisual() {
        if (!_drawPileNode) return;

        // 1. 先清空容器里的旧图片
        _drawPileNode->removeAllChildren();

        int count = _drawPileModels.size();
        if (count == 0) return; // 没牌就不画了

        // 2. 叠放间隔
        float offsetPerCard = 10.0f; //


        for (int i = 0; i < count; i++) {
            auto sp = Sprite::create("res/card_general.png");
            sp->setColor(Color3B(200, 200, 200)); // 背面颜色 (稍微变暗)

            // 每一张都往上挪一点点
            // i=0 是最底下的牌，i=count-1 是最上面的牌
            sp->setPosition(Vec2(0, i * offsetPerCard));

            _drawPileNode->addChild(sp);
        }

        // 3. 更新剩余张数文字
        // 为了不挡住牌，把文字显示在牌堆的最下方 (y = -50)
        auto label = Label::createWithSystemFont(std::to_string(count), "Arial", 24);
        label->setPosition(Vec2(0, -50));
        label->setTextColor(Color4B::WHITE);
        label->enableBold();
        _drawPileNode->addChild(label);
    }

    void GameController::onCardClicked(CardView* clickedView) {
        if (!_topStackCard) return;

        CardModel* clickedCard = clickedView->getModel();
        int clickedFace = clickedCard->getFace();
        int stackFace = _topStackCard->getFace();

        // 判断规则：大1 或 小1
        int diff = std::abs(clickedFace - stackFace);
        bool isMatch = (diff == 1);
        // A(1) 和 K(13) 互通
        if ((clickedFace == 1 && stackFace == 13) || (clickedFace == 13 && stackFace == 1)) {
            isMatch = true;
        }

        if (isMatch) {
            CCLOG("Match! %d -> %d", clickedFace, stackFace);

            // --- A. 记录 Undo 操作 ---
            if (_undoManager) {
                _undoManager->pushOperation(
                    clickedCard,
                    clickedView->getPosition(),
                    clickedView->getLocalZOrder(),
                    _topStackCard
                );
            }

            // --- B. 动画效果 ---
            Vec2 stackPos = POS_BOTTOM_PILE;

            auto moveAction = MoveTo::create(0.3f, stackPos);
            auto scaleAction = ScaleTo::create(0.3f, 1.0f);
            auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

            auto callback = CallFunc::create([this, clickedView, clickedCard]() {
                this->_topStackCard = clickedCard;
                clickedView->setLocalZOrder(100);
                });

            clickedView->runAction(Sequence::create(spawn, callback, nullptr));

        }
        else {
            // 失败抖动
            auto moveBy = MoveBy::create(0.05f, Vec2(10, 0));
            clickedView->runAction(Sequence::create(moveBy, moveBy->reverse(), nullptr));
        }
    }

    // --- 回退功能 ---
    void GameController::onUndoClicked() {
        if (!_undoManager || !_undoManager->canUndo()) {
            return;
        }

        UndoModel* op = _undoManager->popLastOperation();

        // 【关键修复 1】手动 retain，防止在动画播放期间对象被自动销毁
        op->retain();

        // 找到对应的 View
        CardView* view = nullptr;
        for (auto node : _gameView->getChildren()) {
            auto v = dynamic_cast<CardView*>(node);
            if (v && v->getModel() == op->movedCard) {
                view = v;
                break;
            }
        }

        if (view) {
            view->stopAllActions();

            // 动画：飞回原位
            auto moveBack = MoveTo::create(0.3f, op->originalPos);
            // 配合备用牌动画，如果是从备用牌堆出来的，可能需要缩小回去
            auto scaleBack = ScaleTo::create(0.3f, (op->originalPos == POS_DRAW_PILE) ? 0.1f : 1.0f);
            auto spawn = Spawn::create(moveBack, scaleBack, nullptr);

            auto callback = CallFunc::create([this, view, op]() {
                // 1. 恢复层级
                view->setLocalZOrder(op->originalZOrder);

                // 2. 恢复上一张底牌
                this->_topStackCard = op->previousTopCard;

                // 特殊处理：如果这张牌原本来自备用牌堆
                if (op->originalPos == POS_DRAW_PILE) {
                    this->_drawPileModels.pushBack(op->movedCard);
                    view->removeFromParent();

                    // 【新增】退回来后，刷新一下厚度，让它看起来变厚了
                    this->_updateDrawPileVisual();
                }

                // 任务完成，手动释放内存
                op->release();
                });

            view->runAction(Sequence::create(spawn, callback, nullptr));
        }
        else {
            // 如果找不到 View，也要释放 op，防止内存泄漏
            op->release();
        }
    }
}