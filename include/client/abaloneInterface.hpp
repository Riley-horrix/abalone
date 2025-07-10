/**
 * @file abaloneInterface.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Interface for a client controller.
 * @version 0.1.0
 * @date 2025-07-10
 * 
 * Copyright (c) Riley Horrix 2025
 */
#ifndef ABALONE_INTERFACE_H
#define ABALONE_INTERFACE_H

#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace AbaloneInterface {

enum GamePlayer {
    WHITE,
    BLACK
};
    
struct JoinGameInfo {
    GamePlayer side;
    std::string start;
    std::string other_player;
};

struct GamePosition {
    char horizontal;
    char diagonal;
};

struct GamePiece {
    GamePosition position;
    GamePlayer player;
};

struct GameState {
    std::vector<GamePiece> pieces;
};

struct InlineMove {
    GamePosition last;
    GamePosition move;
};

struct BroadsideMove {
    GamePosition first;
    GamePosition last;
    GamePosition firstMove;
};

struct Concede {};

using GameMove = std::variant<InlineMove, BroadsideMove, Concede>;

struct MoveResponse {
    bool success;
    GameMove move;
    std::string reason;
    int reasonId;
};

struct GameFinished {
    bool won;
    std::string winner;
};

struct GameCancelled {};

using MakeMoveResponse = std::variant<MoveResponse, GameFinished, GameCancelled>;

class AbaloneClient {
public:
    std::string getName(void);
    bool requestGame(void);
    bool joinGame(std::unique_ptr<JoinGameInfo> gameInfo);
    void gameStarted(std::unique_ptr<GameState> gameState);
    std::unique_ptr<GameMove> makeMove(std::unique_ptr<GameState> gameState);
    void makeMoveResponse(std::unique_ptr<MakeMoveResponse> response);
};

class AbaloneInterface {
public:
    AbaloneInterface(std::shared_ptr<AbaloneClient> client);
    AbaloneInterface(std::unique_ptr<AbaloneClient> client);
    AbaloneInterface(AbaloneClient* client);
};

}

#endif