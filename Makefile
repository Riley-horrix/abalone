DEBUG?=""

all: build/game

game: build/game
	./build/abalone_game;

interface: build/interface

clean:
	@rm -rf build/*

build/game: build/common
	cd build && cmake --build . --target abalone_game

build/common:
	mkdir -p ./build
	@ if [ "$(DEBUG)" !=  "" ]; then \
		cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..; \
	else \
		cd build && cmake -DCMAKE_BUILD_TYPE=Release ..; \
	fi

build/interface: build/common include/client/abaloneInterface.hpp
	mkdir -p build/interface
	@mv include/client/abaloneInterface.hpp build/interface/




.PHONY: all clean game build/common build/game build/interface