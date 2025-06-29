DEBUG?=""

all: build/game

game: build/game
	@ if [ "$(DEBUG)" !=  "" ]; then \
		./build/abalone_game_debug; \
	else \
		./build/abalone_game; \
	fi

clean:
	@rm -rf build/*

build/game: build/common
	cd build && cmake --build . --target abalone_game
	@ if [ "$(DEBUG)" !=  "" ]; then \
		mv build/abalone_game build/abalone_game_debug ; \
	fi

build/common:
	mkdir -p ./build
	@ if [ "$(DEBUG)" !=  "" ]; then \
		cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..; \
	else \
		cd build && cmake -DCMAKE_BUILD_TYPE=Release ..; \
	fi

.PHONY: all clean game build/common build/game