#include "game.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int seed = 0;
	for (int i = 1; i < argc; i++) {
		std::string cmd = argv[i];  // gets command
		seed = std::stoi(argv[1]);   // gets seed from user, assuming the user will always provide one
	}
	// there is only one game, so good to use shared pointer
	std::shared_ptr<Game> game = std::make_shared<Game>(seed);
	game->startGame();
}
