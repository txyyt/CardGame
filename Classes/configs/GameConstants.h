#pragma once
#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

namespace CardGame {

    // 文档 P5: 花色类型定义
    enum CardSuitType {
        CST_NONE = -1,
        CST_CLUBS = 0,    // 梅花 (Black)
        CST_DIAMONDS,     // 方块 (Red)
        CST_HEARTS,       // 红桃 (Red)
        CST_SPADES,       // 黑桃 (Black)
        CST_NUM_CARD_SUIT_TYPES
    };

    // 文档 P6: 点数类型定义
    enum CardFaceType {
        CFT_NONE = -1,
        CFT_ACE = 1,      // A
        CFT_TWO,
        CFT_THREE,
        CFT_FOUR,
        CFT_FIVE,
        CFT_SIX,
        CFT_SEVEN,
        CFT_EIGHT,
        CFT_NINE,
        CFT_TEN,
        CFT_JACK,         // J
        CFT_QUEEN,        // Q
        CFT_KING,         // K
        CFT_NUM_CARD_FACE_TYPES
    };
}

#endif // __GAME_CONSTANTS_H__