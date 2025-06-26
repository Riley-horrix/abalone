all: build/common build/game

game: build/common build/game
	./build/abalone_game

build/game:
	cd build && cmake --build . --target abalone_game

build/common:
	mkdir -p ./build
	cd build && cmake ..