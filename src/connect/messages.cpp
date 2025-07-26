/**
 * @file messages.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief API messages implementation.
 * @version 0.1
 * @date 2025-07-12
 *
 * Copyright (c) Riley Horrix 2025
 */
#include <sstream>
#include <string>
#include <format>

#include "connect/messages.hpp"
#include "common/utils.hpp"

#include "lib/json.hpp"

using namespace Abalone;

APIVersion::APIVersion(const std::string& version) {
    std::stringstream ss(version);
    char dot1, dot2;

    if (!(ss >> major >> dot1 >> minor >> dot2 >> patch && dot1 == '.' && dot2 == '.')) {
        major = ABALONE_API_VERSION_INVALID;
        minor = ABALONE_API_VERSION_INVALID;
        patch = ABALONE_API_VERSION_INVALID;
    }
}

std::string APIVersion::toString(void) const {
    return std::format("{}.{}.{}", major, minor, patch);
}

bool APIVersion::compatible(void) const {
    if (major != ABALONE_API_VERSION_MAJOR) { return false; }
    if (minor != ABALONE_API_VERSION_MINOR) {
        info("APIVersion: Compatable but different API version being used.");
    }

    return true;
}

// Static message id definitions
const int APIConnect::ID;

// Message implementations

// APIMessage

std::string APIMessage::toString() {
    nlohmann::json j;
    toJSON(j);
    return j.dump();
}

std::unique_ptr<APIMessage> APIMessage::fromString(const std::string& message) {
    nlohmann::json j;

    const auto json = j.parse(message);
    const int id = Utils::getOrDefault(json, "id", -1);

    if (id <= 0) { return nullptr; }

    switch (id) {
        case APIConnect::ID:
            return APIConnect::fromJSON(json);
    }

    return nullptr;
}

nlohmann::json APIMessage::formatGameState(const GameState& state) {
    nlohmann::json arr = nlohmann::json::array();

    for (const auto pos : state.positions) {
        arr.push_back(formatGamePosition(pos));
    }

    return arr;
}

nlohmann::json APIMessage::formatGamePosition(const APIPlayerPosition& position){
    nlohmann::json json;

    std::string positionStr = std::format("{}{}", position.position.horizontal, position.position.diagonal);
    std::string pieceStr = position.player == Player::WHITE ? "w" :
                        position.player == Player::BLACK ? "b" :
                        "x";

    json["pos"] = positionStr;
    json["piece"] = pieceStr;

    return json;
}

bool APIMessage::gameStateFromJson(const nlohmann::json& json, GameState& position) {

    std::vector<nlohmann::json> stateArray = Utils::getOrDefault<std::vector<nlohmann::json>>(json, "game_state", {});

    if(stateArray.size() == 0){ return false; }

    std::vector<APIPlayerPosition> positions;

    APIPlayerPosition playerPos = {Player::NONE, Position::invalid()};

    for (const auto& position : stateArray) {
        if(gamePositionFromJson(position, playerPos)) {
            positions.push_back(playerPos);
        }
    }

    position = GameState(positions);
    return true;
}

bool APIMessage::gamePositionFromJson(const nlohmann::json& json, APIPlayerPosition& position) {
    std::string gamePosition;
    std::string gamePiece;

    if ((gamePosition = Utils::getOrDefault<std::string>(json, "pos", "")) == ""){ return false; }
    if ((gamePosition = Utils::getOrDefault<std::string>(json, "piece", "")) == ""){ return false; }

    if (gamePosition.size() != 2) {
        return false;
    }

    if (gamePiece.size() != 1) {
        return;
    }

    Position pos = Position(gamePosition[0], gamePosition[1]);

    if (!pos.isValid()) {
        return false;
    }

    Player player = 
        gamePiece == "w" ? Player::WHITE :
        gamePiece == "b" ? Player::BLACK :
        Player::NONE;

    if (player == Player::NONE) {
        return false;
    }

    position = {player, pos};

    return true;
}

APIMessage::APIMessage(int id): id(id) {}
APIMessage::~APIMessage() {}

// APIConnect

std::unique_ptr<APIMessage> APIConnect::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string name;
    if ((name = Utils::getOrDefault<std::string>(j["content"], "name", "")) == "") { return nullptr; }
    std::string version;
    if ((version = Utils::getOrDefault<std::string>(j["content"], "version", "")) == "") { return nullptr; }
    std::string magic;
    if ((magic = Utils::getOrDefault<std::string>(j["content"], "magic", "")) == "") { return nullptr; }

    // Check magic
    if (magic != ABALONE_API_CONNECT_MAGIC) { return nullptr; }
    // Check version compatible
    APIVersion apiVersion(version);
    if (!apiVersion.compatible()) { return nullptr; }

    return std::make_unique<APIConnect>(name, apiVersion);
}

void APIConnect::toJSON(nlohmann::json& json) const {
    json["id"] = APIConnect::ID;
    json["content"] = {
        {"version", version.toString()},
        {"magic", magic},
        {"name", name}
    };
}

// APIAck

std::unique_ptr<APIMessage> APIAck::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string name;
    if ((name = Utils::getOrDefault<std::string>(j["content"], "name", "")) == "") { return nullptr; }
    std::string version;
    if ((version = Utils::getOrDefault<std::string>(j["content"], "version", "")) == "") { return nullptr; }

    // Check version compatible
    APIVersion apiVersion(version);
    if (!apiVersion.compatible()) { return nullptr; }

    return std::make_unique<APIAck>(name, apiVersion);
}


void APIAck::toJSON(nlohmann::json& json) const {
    json["id"] = APIAck::ID;
    json["content"] = {
        {"name", name}
    };
}

// APIRequestGame

std::unique_ptr<APIMessage> APIRequestGame::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }


    std::string sideStr;
    if ((sideStr = Utils::getOrDefault<std::string>(j["content"], "desired_side", "")) == "") { return nullptr; }

    // Default to NONE on bad entry
    PlayerSide side = 
        sideStr == "black" ? PlayerSide::BLACK :
        sideStr == "white" ? PlayerSide::WHITE :
        sideStr == "random" ? PlayerSide::RANDOM :
        PlayerSide::NONE;
    
    std::string startStr;
    if ((startStr = Utils::getOrDefault<std::string>(j["content"], "desired_start", "")) == "") { return nullptr; }

    // Default to NONE on bad entry
    GameStart start =
        startStr == "belgian" ? GameStart::BELGIAN :
        startStr == "german" ? GameStart::GERMAN :
        startStr == "random" ? GameStart::RANDOM :
        GameStart::NONE;

    return std::make_unique<APIRequestGame>(side, start);
}

void APIRequestGame::toJSON(nlohmann::json& json) const {
    json["id"] = APIRequestGame::ID;
    json["content"] = {
        {"desired_side", side},
        {"desired_start", start}
    };
}

// APIJoinGame

std::unique_ptr<APIMessage> APIJoinGame::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string otherPlayer;
    if ((otherPlayer = Utils::getOrDefault<std::string>(j["content"], "other_player", "")) == "") { return nullptr; }
    
    return std::make_unique<APIJoinGame>(otherPlayer);
}

void APIJoinGame::toJSON(nlohmann::json& json) const {
    json["id"] = APIJoinGame::ID;
    json["content"] = {
        {"other_player", otherPlayer}
    };
}

// APIAcceptGame

std::unique_ptr<APIMessage> APIAcceptGame::fromJSON(const nlohmann::json& j) {    
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    return std::make_unique<APIAcceptGame>();
}

void APIAcceptGame::toJSON(nlohmann::json& json) const {
    json["id"] = APIAcceptGame::ID;
    json["content"] = {};
}

// APIGameStart

std::unique_ptr<APIMessage> APIGameStart::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    // Extract game state from json
    GameState state({});

    if (!gameStateFromJson(j["content"], state)) {
        return nullptr;
    }

    return std::make_unique<APIGameStart>(state);
}

void APIGameStart::toJSON(nlohmann::json& json) const {
    json["id"] = APIGameStart::ID;
    json["content"] = {
        {"game_state", APIMessage::formatGameState(state)}
    };
}

// APIGameCancelled

std::unique_ptr<APIMessage> APIGameCancelled::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string reasonStr;
    if ((reasonStr = Utils::getOrDefault<std::string>(j["content"], "reason", "")) == "") { return nullptr; }

    int reasonId;
    if ((reasonId = Utils::getOrDefault<int>(j["content"], "id", -1)) == -1) { return nullptr; }

    return std::make_unique<APIGameCancelled>(reasonStr, reasonId);
}

void APIGameCancelled::toJSON(nlohmann::json& json) const {
    json["id"] = APIGameCancelled::ID;
    json["content"] = {
        {"reason", reason},
        {"id", reasonId}
    };
}

// APIRequestMove

std::unique_ptr<APIMessage> APIRequestMove::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    // Extract game state from json
    GameState state({});

    if (!gameStateFromJson(j["content"], state)) {
        return nullptr;
    }

    return std::make_unique<APIRequestMove>(state);
}

void APIRequestMove::toJSON(nlohmann::json& json) const {
    json["id"] = APIRequestMove::ID;
    json["content"] = {
        {"game_state", APIMessage::formatGameState(state)}
    };
}

// APIRequestGameState

std::unique_ptr<APIMessage> APIRequestGameState::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    return std::make_unique<APIRequestGameState>();
}

void APIRequestGameState::toJSON(nlohmann::json& json) const {
    json["id"] = APIRequestGameState::ID;
    json["content"] = {};
}

// APIGameState

std::unique_ptr<APIMessage> APIGameState::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    // Extract game state from json
    GameState state({});

    if (!gameStateFromJson(j["content"], state)) {
        return nullptr;
    }

    return std::make_unique<APIGameState>(state);
}

void APIGameState::toJSON(nlohmann::json& json) const {
    json["id"] = APIGameState::ID;
    json["content"] = {
        {"game_state", APIMessage::formatGameState(state)}
    };
}

// APIInlineMove

std::unique_ptr<APIMessage> APIInlineMove::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string last;
    if ((last = Utils::getOrDefault<std::string>(j["content"], "last", "")) == "") { return nullptr; }

    std::string move;
    if ((move = Utils::getOrDefault<std::string>(j["content"], "move", "")) == "") { return nullptr; }

    if (last.size() != 2) {
        return nullptr;
    }

    if (move.size() != 2) {
        return nullptr;
    }

    Position lastPos(last[0], last[1]);
    Position movePos(move[0], move[1]);

    if (!lastPos.isValid() || !movePos.isValid()) {
        return nullptr;
    }

    return std::make_unique<APIInlineMove>(lastPos, movePos);
}

void APIInlineMove::toJSON(nlohmann::json& json) const {
    json["id"] = APIInlineMove::ID;
    json["content"] = {
        {"last", APIMessage::formatGamePosition(last)},
        {"move", APIMessage::formatGamePosition(move)}
    };
}

// APIBroadsideMove

std::unique_ptr<APIMessage> APIBroadsideMove::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    std::string first;
    if ((first = Utils::getOrDefault<std::string>(j["content"], "first", "")) == "") { return nullptr; }

    std::string last;
    if ((last = Utils::getOrDefault<std::string>(j["content"], "last", "")) == "") { return nullptr; }

    std::string firstMove;
    if ((firstMove = Utils::getOrDefault<std::string>(j["content"], "first_move", "")) == "") { return nullptr; }

    if (first.size() != 2) {
        return nullptr;
    }

    if (last.size() != 2) {
        return nullptr;
    }

    if (firstMove.size() != 2) {
        return nullptr;
    }

    Position firstPos(first[0], first[1]);
    Position lastPos(last[0], last[1]);
    Position firstMovePos(firstMove[0], firstMove[1]);

    if (!firstPos.isValid() || !lastPos.isValid() || !firstMovePos.isValid()) {
        return nullptr;
    }

    return std::make_unique<APIBroadsideMove>(firstPos, lastPos, firstMovePos);
}

void APIBroadsideMove::toJSON(nlohmann::json& json) const {
    json["id"] = APIBroadsideMove::ID;
    json["content"] = {
        {"first", APIMessage::formatGamePosition(first)},
        {"last", APIMessage::formatGamePosition(last)},
        {"first_move", APIMessage::formatGamePosition(firstMove)}
    };
}

// APIMoveAccepted

std::unique_ptr<APIMessage> APIMoveAccepted::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }
    if (!j["content"].contains("message") || !j["content"]["message"].is_object()) { return nullptr; }

    // Validate the message ID for inline or broadside moves
    nlohmann::json msgJson = j["content"]["message"];

    int msgId = Utils::getOrDefault(msgJson, "id", -1);

    if (msgId == -1) {
        return nullptr;
    }

    if (msgId != APIBroadsideMove::ID && msgId != APIInlineMove::ID) {
        return nullptr;
    }

    std::unique_ptr<APIMessage> msg = APIMessage::fromJSON(msgJson);

    if (msg == nullptr) {
        return nullptr;
    }

    return std::make_unique<APIMoveAccepted>(std::move(msg));
}

void APIMoveAccepted::toJSON(nlohmann::json& json) const {
    json["id"] = APIMoveAccepted::ID;

    nlohmann::json content;

    // Use override toJSON of stored message
    nlohmann::json msgJson;
    if (msg->id == APIInlineMove::ID) {
        const APIInlineMove* inMove = static_cast<APIInlineMove*>(msg.get());
        inMove->toJSON(msgJson);
    } else if (msg->id == APIBroadsideMove::ID) {
        const APIBroadsideMove* broadMove = static_cast<APIBroadsideMove*>(msg.get());
        broadMove->toJSON(msgJson);
    } else {
        error("Messages: Attempted to jsonify an api move accepted with message id: %d", msg->id);
        json["content"] = {};
        return;
    }

    content["message"] = msgJson;
    json["content"] = content;
}

// APIMoveRejected

std::unique_ptr<APIMessage> APIMoveRejected::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }
    if (!j["content"].contains("message") || !j["content"]["message"].is_object()) { return nullptr; }

    // Validate the message ID for inline or broadside moves
    nlohmann::json msgJson = j["content"]["message"];

    int msgId = Utils::getOrDefault(msgJson, "id", -1);

    if (msgId == -1) {
        return nullptr;
    }

    if (msgId != APIBroadsideMove::ID && msgId != APIInlineMove::ID) {
        return nullptr;
    }

    std::unique_ptr<APIMessage> msg = APIMessage::fromJSON(msgJson);

    if (msg == nullptr) {
        return nullptr;
    }

    std::string reasonStr;
    int reasonId;

    if ((reasonStr = Utils::getOrDefault<std::string>(j["content"], "reason", "")) != "") {
        return nullptr;
    }

    if ((reasonId = Utils::getOrDefault<int>(j["content"], "id", -1)) == -1) {
        return nullptr;
    }

    return std::make_unique<APIMoveRejected>(std::move(msg), reasonStr, reasonId);
}

void APIMoveRejected::toJSON(nlohmann::json& json) const {
    json["id"] = APIMoveRejected::ID;

    nlohmann::json content;
    content["reason"] = reason;
    content["id"] = reasonId;

    // Use override toJSON of stored message
    nlohmann::json msgJson;
    if (msg->id == APIInlineMove::ID) {
        const APIInlineMove* inMove = static_cast<APIInlineMove*>(msg.get());
        inMove->toJSON(msgJson);
    } else if (msg->id == APIBroadsideMove::ID) {
        const APIBroadsideMove* broadMove = static_cast<APIBroadsideMove*>(msg.get());
        broadMove->toJSON(msgJson);
    } else {
        error("Messages: Attempted to jsonify an api move accepted with message id: %d", msg->id);
        json["content"] = {};
        return;
    }

    content["message"] = msgJson;
    json["content"] = content;
}

// APIGameFinished

std::unique_ptr<APIMessage> APIGameFinished::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    if (!j["content"].contains("won") || !j["content"]["won"].is_boolean()) { return nullptr; }
    bool won = j["content"]["won"];

    return std::make_unique<APIGameFinished>(won);
}

void APIGameFinished::toJSON(nlohmann::json& json) const {
    json["id"] = APIGameFinished::ID;
    json["content"] = {
        {"won", won}
    };
}

// APIConcede

std::unique_ptr<APIMessage> APIConcede::fromJSON(const nlohmann::json& j) {
    if (!j.contains("content") || !j["content"].is_object()) { return nullptr; }

    return std::make_unique<APIConcede>();
}

void APIConcede::toJSON(nlohmann::json& json) const {
    json["id"] = APIConcede::ID;
    json["content"] = {};
}
