#include "LevelConfigLoader.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "cocos2d.h"

using namespace cocos2d;

namespace CardGame {

    LevelConfigData LevelConfigLoader::loadLevelConfig(int levelId) {
        LevelConfigData configData;

        // 1. 拼接文件名，例如 "level1.json"
        std::string filename = StringUtils::format("level%d.json", levelId);

        // 2. 利用 Cocos 的 FileUtils 读取文件内容
        std::string content = FileUtils::getInstance()->getStringFromFile(filename);

        if (content.empty()) {
            CCLOG("Error: Failed to load level config: %s", filename.c_str());
            return configData;
        }

        // 3. 解析 JSON
        rapidjson::Document doc;
        doc.Parse(content.c_str());

        if (doc.HasParseError()) {
            CCLOG("Error: JSON parse error in %s", filename.c_str());
            return configData;
        }

        // 4. 读取 Playfield 数组
        if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
            const rapidjson::Value& playfieldArr = doc["Playfield"];
            for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); i++) {
                const rapidjson::Value& item = playfieldArr[i];

                CardConfigData card;
                if (item.HasMember("CardFace")) card.cardFace = item["CardFace"].GetInt();
                if (item.HasMember("CardSuit")) card.cardSuit = item["CardSuit"].GetInt();
                if (item.HasMember("Position")) {
                    const rapidjson::Value& pos = item["Position"];
                    card.position = Vec2(pos["x"].GetFloat(), pos["y"].GetFloat());
                }

                configData.playfieldCards.push_back(card);
            }
        }

        // 5. 读取 Stack 数组
        if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
            const rapidjson::Value& stackArr = doc["Stack"];
            for (rapidjson::SizeType i = 0; i < stackArr.Size(); i++) {
                const rapidjson::Value& item = stackArr[i];

                CardConfigData card;
                if (item.HasMember("CardFace")) card.cardFace = item["CardFace"].GetInt();
                if (item.HasMember("CardSuit")) card.cardSuit = item["CardSuit"].GetInt();
                if (item.HasMember("Position")) {
                    const rapidjson::Value& pos = item["Position"];
                    card.position = Vec2(pos["x"].GetFloat(), pos["y"].GetFloat());
                }

                configData.stackCards.push_back(card);
            }
        }

        return configData;
    }
}