#include "game.h"
#include <sstream>
#include <iterator>

// Normal constructor
Game::Game(int seed)
	: cardeck{Deck{seed}},
	  table{Table{}},
	  curplayer{0}
{
	for (int i = 0; i < 4; i++) {
		std::cout << "Is Player" << i + 1 << " a human (h) or a computer (c)?" 
			<< std::endl;
		std::string type;
		std::getline(std::cin, type);
		addPlayer(type);
	}
}

// Destructor
Game::~Game() {}

// cardeck getter
Deck Game::getCardeck() const {
	return cardeck;
}

// table getter
Table Game::getTable() const {
	return table;
}

// create and adds a computer or human player to the list of players
void Game::addPlayer(std::string type) {
	if (players.size() >= 4) {
		return;
	}
	if (type == "h") {
		Player * p = new HumanPlayer();
		p->attach(&table);
		players.emplace_back(p);
		return;
	}
	else {
		Player * p = new ComputerPlayer();
		p->attach(&table);
		players.emplace_back(p);
		return;
	}
}

// checks if the game is over
bool Game::gameOver() {
	for (int i = 0; i < 4; i++) {
		// if a player has score over 80, game over
		if (players[i]->getOldscore() >= 80) {
			return true;
		}
	}
	return false;
}

// prints each player's state at the end of each round
void Game::printState() {
	for (int i = 0; i < 4; i++) {
		// print all discards
		std::cout << "Player" << i + 1 << "'s discards:";
		players[i]->printCards(players[i]->getDiscards());
		// print scores
		int old = players[i]->getOldscore();
		players[i]->setScore();
		int gained = players[i]->getScore();
		std::cout << "Player" << i + 1 << "'s score: " << old << " + " <<
			gained << " = ";
		players[i]->setOldscore(old + gained);
		std::cout << players[i]->getOldscore() << std::endl;
	}
}

// starts the game and play until game is over
void Game::startGame() {
	while(!gameOver()) {
		newRound();
		printState();
	}
	printWinners();
}

// checks if a round is over
bool Game::roundOver() {
	// a counter for checking if each player's hand is empty
	bool count = true;
	for (int i = 0; i < 4; i++) {
		count = count && players[i]->isEmpty();
	}
	return count;
}

// deals 13 cards to each player
void Game::dealCards() {
	for (int i = 0; i < 4; i++) {
		players[i]->setHand(cardeck.getFulldeck(i*13, i*13 + 13));
	}
}

// starts a new round and play until the round ends
void Game::newRound() {
	int first = 0;
	cardeck.shuffle();
	table.clearTable();
	for (int i = 0; i < 4; i++) {
		players[i]->reset();
	}
	dealCards();
	// find the player that has 7S
	bool found = false;
	for (int i = 0; i < 4; i++) {
		Card c{SPADE, SEVEN};
		for (unsigned int j = 0; j < players[i]->getHand().size(); j++) {
		    if (*(players[i]->getHand()[j]) == c) {
		        first = i;
		        curplayer = i;
		        found = true;
		        break;
		    }
		}
		if (found) {
		    break;
		}
	}
	std::cout << "A new round begins. It's Player" << first + 1 << "'s turn to play" << std::endl;
	// play until the round is over
	while (!roundOver()) {
		play();
		if (curplayer < 3) {
			curplayer += 1;
		}
		else {
			curplayer = 0;
		}
	}

}

// prints information for each turn
void Game::printTurn() {
	// prints all cards on the table
	table.printTable();
	std::cout << "Your hand:";
	// prints all cards a player currently holds
	players[curplayer]->printCards(players[curplayer]->getHand());
	// print legal plays
	std::cout << "Legal plays:";
	players[curplayer]->printCards(players[curplayer]->getCurLegal());
}

// facilitate the turns
void Game::play() {
	// find legal plays and display them
	players[curplayer]->setCurLegal(table);
	printTurn();
	// if current player is a computer player
	if (players[curplayer]->playertype() == C) {
		std::cout << "Player" << curplayer + 1 << " plays ";
		dynamic_cast<ComputerPlayer *>(players[curplayer])->playCard();
	}
	else {
		// human player, ask for commands
		std::string s;
		while (std::getline(std::cin, s)) {
			std::istringstream is{s};
			std::string cmd;
			is >> cmd;

			// player command to play
			if (cmd == "play") {
				is >> cmd; // get the card commanded to playa
				std::cout << "Player" << curplayer + 1 << " plays ";
				dynamic_cast<HumanPlayer *>(players[curplayer])->playCard(cmd);
				return;
			}
			// player command to discard
			else if (cmd == "discard") {
				// if there is legal plays, not allowed to discard
				if (players[curplayer]->getCurLegal().size() > 0) {
					std::cout << "You have a legal play. You may not discard." << std::endl;
				}
				else {
					is >> cmd;  // get the card commanded to discarda
					std::cout << "Player" << curplayer + 1 << " discards ";
					dynamic_cast<HumanPlayer *>(players[curplayer])->discardCard(cmd);
					return;
				}
			}
			// player command to print the full deck
			else if (cmd == "deck") {
				cardeck.printDeck();
			}
			// command to quit
			else if (cmd == "quit") {
				printState();
				printWinners();
				exit(0);
			}
			// rage quit
			else if (cmd == "ragequit") {
				std::cout << "Player" << curplayer + 1 << " ragequits. A computer will now take over." 
					<< std::endl;
				std::vector<Card *> h = players[curplayer]->getHand();
				std::vector<Card *> d = players[curplayer]->getDiscards();
				int os = players[curplayer]->getOldscore();
				Card * c = players[curplayer]->getCur();
				delete players[curplayer];
				players[curplayer] = new ComputerPlayer();
				players[curplayer]->setHand(h);
				players[curplayer]->setDiscards(d);
				//newp->setScore(players[curplayer]->getScore());
				players[curplayer]->setOldscore(os);
				players[curplayer]->setCur(c);
				
				dynamic_cast<ComputerPlayer *>(players[curplayer])->playCard();
			}
			else {
				std::cout << "Invalid command" << std::endl;
			}
		}
	}
}

// prints the winner
void Game::printWinners() {
	int winscore = 10000;
	for (int i = 0; i < 4; i++) {
		if (players[i]->getOldscore() < winscore) {
			winscore = players[i]->getOldscore();
		}
	}
	for (int i = 0; i < 4; i++) {
		if (players[i]->getOldscore() == winscore) {
			std::cout << "Player" << i + 1 << " wins!" << std::endl;
		}
	}
}
