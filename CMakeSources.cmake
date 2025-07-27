set(SRC_FILES 
    # Game
    ./src/game/board.cpp
    ./src/game/move.cpp
    ./src/game/position.cpp

    # Common
    ./src/common/utils.cpp
    ./src/common/logging.cpp

    # Connect
    ./src/connect/messages.cpp
    ./src/connect/apiMessageStream.cpp
    ./src/connect/tcpServer.cpp
    ./src/connect/socketStream.cpp

    # Graphics
    ./src/graphics/renderer.cpp
    ./src/graphics/textRenderer.cpp
)
