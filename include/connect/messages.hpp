/**
 * @file messages.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief API messages definitions.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <string>

#include "common/logging.hpp"
#include "common/utils.hpp"
#include "game/board.hpp"
#include "game/position.hpp"

#include "lib/json.hpp"

#define ABALONE_API_VERSION_MAJOR 0
#define ABALONE_API_VERSION_MINOR 1
#define ABALONE_API_VERSION_PATCH 0
#define ABALONE_API_VERSION_INVALID -1

#define ABALONE_API_CONNECT_MAGIC "ABALONE"

namespace Abalone {

/**
 * @brief Struct to hold version information.
 */
struct APIVersion {
    /// @brief Major version number.
    int major;

    /// @brief Minor version number.
    int minor;
    
    /// @brief Patch version number.
    int patch;

    /**
     * @brief Construct a new APIVersion object from a version string.
     * 
     * @param version Version string in the form "%d.%d.%d".
     */
    APIVersion(const std::string& version);

    /**
     * @brief Transform the version object into a string.
     * 
     * @return std::string Version string in the form "%d.%d.%d".
     */
    std::string toString(void) const;

    /**
     * @brief Returns if the APIVersion is compatible with the current compiled version.
     * 
     * @return bool If the version is valid. 
     */
    bool compatible(void) const;
};


/// @brief Starting side.
enum class PlayerSide {
    BLACK = static_cast<int>(Player::BLACK),
    WHITE = static_cast<int>(Player::WHITE),
    NONE = static_cast<int>(Player::NONE),
    RANDOM
};


/// @brief Starting layout.
enum class GameStart {
    BELGIAN = static_cast<int>(GameOpening::BELGIAN_DAISY),
    GERMAN = static_cast<int>(GameOpening::BELGIAN_DAISY),
    NONE,
    RANDOM
};


struct APIPlayerPosition {
    Player player;
    Position position;
};


struct GameState {
    /**
     * @brief Construct a new GameState object.
     * 
     * @param positions List of game options.
     */
    GameState(std::vector<APIPlayerPosition> positions): positions(positions) {}

    /// @brief List of game positions.
    std::vector<APIPlayerPosition> positions;
};


class APIMessage {
public:
    /// @brief Message identifier.
    const int id;

    /**
     * @brief Convert the message to a JSON formatted string.
     * 
     * @return std::string The JSON formatted string.
     */
    std::string toString();

    /**
     * @brief Construct an API message from a string.
     * 
     * @return APIMessage Unique pointer to the API message. Returns nullptr if the string is invalid.
     */
    static std::unique_ptr<APIMessage> fromString(const std::string& message);

    /**
     * @brief Get the Current Version object.
     * 
     * @return std::string 
     */
    static inline constexpr const char* getCurrentVersion(void) {
        return STR(ABALONE_API_VERSION_MAJOR) "." STR(ABALONE_API_VERSION_MINOR) "." STR(ABALONE_API_VERSION_PATCH);
    }

    /**
     * @brief Destroy the APIMessage object.
     */
    virtual ~APIMessage();

protected:
    // Private constructor
    APIMessage(int id);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    virtual void toJSON(nlohmann::json& json) const = 0;

    /**
     * @brief Translate a json object into a message.
     * 
     * @param json The json object.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Format a GameState into the API representation.
     * 
     * @param state The Game State.
     * @return nlohmann::json Its API representation.
     */
    static nlohmann::json formatGameState(const GameState& state);

    /**
     * @brief Format a Player position into the API representation.
     * 
     * @param position The player position
     * @return nlohmann::json Its API representation.
     */
    static nlohmann::json formatGamePosition(const APIPlayerPosition& position);

    static bool gameStateFromJson(const nlohmann::json& json, GameState& position);

    static bool gamePositionFromJson(const nlohmann::json& json, APIPlayerPosition& position);
};

/**
 * @brief Message used to connect to a running Abalone instance.
 * 
 */
class APIConnect : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 1;

    /**
     * @brief Construct a new APIConnect object with a username.
     * 
     * @param name The player's name.
     */
    APIConnect(const std::string& name): APIMessage(APIConnect::ID), 
        version(APIMessage::getCurrentVersion()), magic(ABALONE_API_CONNECT_MAGIC), name(name) {}

    /**
     * @brief Construct a new APIConnect object with a username and version number.
     * 
     * @param name The player's name.
     * @param version The message API version.
     */
    APIConnect(const std::string& name, const APIVersion& version): APIMessage(APIConnect::ID),
        version(version), magic(ABALONE_API_CONNECT_MAGIC), name(name) {}

    /// @brief The version of the API being used.
    APIVersion version;

    /// @brief Server magic code.
    std::string magic;

    /// @brief The name of the user connecting.
    std::string name;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to acknowledge a user joining,
 * also sends the server version protocol.
 */
class APIAck : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 2;

    /**
     * @brief Construct a new APIAck object with a username.
     * 
     * @param name The player's name.
     */
    APIAck(const std::string& name): APIMessage(APIAck::ID), 
        version(APIMessage::getCurrentVersion()), name(name) {}

    /**
     * @brief Construct a new APIAck object with a username and version number.
     * 
     * @param name The player's name.
     * @param version The message API version.
     */
    APIAck(const std::string& name, const APIVersion& version): APIMessage(APIAck::ID),
        version(version), name(name) {}

    /// @brief The version of the server API being used.
    APIVersion version;

    /// @brief The name of the user connecting.
    std::string name;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to request that they join a game.
 */
class APIRequestGame : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 3;

    /**
     * @brief Construct a new APIRequestGame object with a username.
     * 
     * @param name The player's name.
     */
    APIRequestGame(PlayerSide side, GameStart start): APIMessage(APIRequestGame::ID),
        side(side), start(start) {}

    /// @brief Starting side for the player.
    PlayerSide side;

    /// @brief Game starting position for the player.
    GameStart start;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to ask them if they would like to join
 * a specific game.
 */
class APIJoinGame : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 4;

    /**
     * @brief Construct a new APIJoinGame object.
     * 
     * @param name The name of the other player.
     */
    APIJoinGame(const std::string& otherPlayer): APIMessage(APIJoinGame::ID),
        otherPlayer(otherPlayer) {}

    /// @brief Name of the other player.
    std::string otherPlayer;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to accept and start playing the 
 * requested game.
 */
class APIAcceptGame : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 5;

    /**
     * @brief Construct a new APIAcceptGame object.
     */
    APIAcceptGame(): APIMessage(APIAcceptGame::ID) {}

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to represent a game starting.
 */
class APIGameStart : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 6;

    /**
     * @brief Construct a new APIGameStart object.
     * 
     * @param state The starting game state.
     */
    APIGameStart(const GameState& state): APIMessage(APIGameStart::ID),
        state(state) {}

    /// @brief The current game state.
    GameState state;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to represent a game starting.
 */
class APIGameCancelled : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 7;

    /**
     * @brief Construct a new APIGameCancelled object.
     * 
     * @param reason The string reason for cancelling the game.
     * @param reasonId The integer id for the reason.
     */
    APIGameCancelled(const std::string& reason, int reasonId): APIMessage(APIGameCancelled::ID),
        reason(reason), reasonId(reasonId) {}

    /// @brief The reason for cancelling the game.
    std::string reason;

    /// @brief The reason identifier.
    int reasonId;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to ask the client to make a move.
 */
class APIRequestMove : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 8;

    /**
     * @brief Construct a new APIRequestMove object.
     */
    APIRequestMove(const GameState& state): APIMessage(APIRequestMove::ID),
        state(state) {}

    /// @brief The current game state.
    GameState state;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to request the game state.
 */
class APIRequestGameState : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 9;

    /**
     * @brief Construct a new APIRequestGameState object.
     */
    APIRequestGameState(): APIMessage(APIRequestGameState::ID) {}

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to indicate the current game state.
 */
class APIGameState : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 10;

    /**
     * @brief Construct a new APIGameState object.
     */
    APIGameState(const GameState& state): APIMessage(APIGameState::ID),
        state(state) {}

    /// @brief The current game state.
    GameState state;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to request an inline move.
 */
class APIInlineMove : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 11;

    /**
     * @brief Construct a new APIInlineMove object.
     * 
     * @param last The position of the last piece in the inline move.
     * @param move The moved position of the last piece in the move.
     */
    APIInlineMove(const APIPlayerPosition& last, const APIPlayerPosition move): APIMessage(APIInlineMove::ID),
        last(last), move(move) {}

    /// @brief The position of the last piece in the inline move.
    APIPlayerPosition last;
    
    /// @brief The position of the last piece when moved.
    APIPlayerPosition move;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to request a broadside move.
 */
class APIBroadsideMove : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 12;

    /**
     * @brief Construct a new APIBroadsideMove object.
     * 
     * @param first The position of the first piece in the broadside move.
     * @param last The position of the last piece in the broadside move.
     * @param firstMove The moved position of the first piece in the broadside move.
     */
    APIBroadsideMove(const APIPlayerPosition& first, const APIPlayerPosition& last, const APIPlayerPosition& firstMove): APIMessage(APIBroadsideMove::ID),
        first(first), last(last), firstMove(firstMove) {}

    /// @brief The position of the first piece in the broadside move.
    APIPlayerPosition first;
    
    /// @brief The position of the last piece in the broadside move.
    APIPlayerPosition last;

    /// @brief The moved position of the first piece in the broadside move.
    APIPlayerPosition firstMove;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to indicate that the client's move
 * was accepted.
 */
class APIMoveAccepted : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 13;

    /**
     * @brief Construct a new APIMoveAccepted object.
     * 
     * @param msg The position of the first piece in the broadside move.
     */
    APIMoveAccepted(std::unique_ptr<APIMessage> msg): APIMessage(APIMoveAccepted::ID),
        msg(std::move(msg)) {}

    /// @brief The move that got accepted.
    std::unique_ptr<APIMessage> msg;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to indicate that the server has 
 * rejected a move for a reason.
 */
class APIMoveRejected : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 14;

    /**
     * @brief Construct a new APIMoveRejected object.
     * 
     * @param msg The position of the first piece in the broadside move.
     */
    APIMoveRejected(std::unique_ptr<APIMessage> msg, const std::string& reason, int reasonId): APIMessage(APIMoveRejected::ID),
        msg(std::move(msg)), reason(reason), reasonId(reasonId) {}

    /// @brief The move that got accepted.
    std::unique_ptr<APIMessage> msg;

    /// @brief The reason for rejecting the move.
    std::string reason;

    /// @brief The integer id for the reason.
    int reasonId;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by server to client to represent the conclusion of a game.
 */
class APIGameFinished : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 15;

    /**
     * @brief Construct a new APIGameFinished object.
     * 
     * @param msg The position of the first piece in the broadside move.
     */
    APIGameFinished(bool won): APIMessage(APIGameFinished::ID),
        won(won) {}

    /// @brief Whether the player won the game or not.
    bool won;

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};


/**
 * @brief Message sent by client to server to represent them conceding the current game.
 */
class APIConcede : public APIMessage {
public:
    /// @brief Message identifier.
    static const int ID = 16;

    /**
     * @brief Construct a new APIConcede object.
     * 
     * @param msg The position of the first piece in the broadside move.
     */
    APIConcede(): APIMessage(APIConcede::ID) {}

    /**
     * @brief Create the message from a JSON object.
     * 
     * Assumes that the "id" field is set and correct.
     * 
     * @param json The JSON object to read from.
     * @return std::unique_ptr<APIMessage> The message, or nullptr if invalid.
     */
    static std::unique_ptr<APIMessage> fromJSON(const nlohmann::json& json);

    /**
     * @brief Convert the message to JSON.
     * 
     * @param json The JSON object to write values into.
     */
    void toJSON(nlohmann::json& json) const override;
};

}