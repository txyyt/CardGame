# Digital Quality Control Card Game (Demo)


## 🛠 开发环境 (Environment)
- **Engine**: Cocos2d-x 3.17.2
- **IDE**: Visual Studio 2026
- **Language**: C++ 11
- **Platform**: Windows (Win32)

## 🚀 如何运行 (How to Run)
为了保持仓库轻量，本项目**未包含** Cocos2d-x 引擎核心文件。请按照以下步骤配置：

1. 克隆本项目到本地。
2. 将现有的 `cocos2d` 引擎文件夹完整复制到项目根目录下。
   - 确保路径结构为：`CardGame/cocos2d/...`
3. 使用 Visual Studio 打开 `proj.win32/CardGame.sln`。
4. 编译并运行 (Debug/x86)。

## ✨ 功能特性 (Features)
- **MVC 架构**: 严格遵循 Model-View-Controller 分层设计。
- **组合视图模式**: 动态拼装卡牌资源（底板+花色+数字）。
- **核心玩法**: 
  - 支持点击消除（大1或小1匹配）。
  - 支持备用牌堆翻牌逻辑。
  - **Undo 系统**: 使用命令模式实现无限回退功能。

## 📂 核心代码目录
- `Classes/configs`: 游戏数值配置与常量定义。
- `Classes/models`: 核心数据模型 (CardModel, UndoModel)。
- `Classes/views`: 视图渲染 (CardView, GameView)。
- `Classes/controllers`: 业务逻辑控制 (GameController)。
- `Classes/managers`: 状态管理 (UndoManager)。