#pragma once
#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "LevelConfig.h"
#include <string>

namespace CardGame {

    class LevelConfigLoader {
    public:
        // 静态函数：传入关卡ID，返回加载好的关卡数据
        // 参数 levelId: 比如 1 代表 level1.json
        static LevelConfigData loadLevelConfig(int levelId);
    };

}

#endif // __LEVEL_CONFIG_LOADER_H__