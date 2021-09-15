#ifndef GAME_H
#define GAME_H
#include "card.h"
#include "deck.h"
#include "player.h"
#include "computerplayer.h"
#include "humanplayer.h"
#include "table.h"
#include "observer.h"
#include "subject.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class Game {
protected:
	Deck cardeck;       // the deck of cards shuffled this round
	Table table;        // holds the current game table
	std::vector<Player *> players;      // holds the 4 players
	int curplayer;                      // the current player
public:
	Game(int seed);         // normal constructor
	~Game();                // Destructor
	Deck getCardeck() const;      // cardeck getter
	Table getTable() const;       // table getter
	void addPlayer(std::string type);       // create a computer or human player
	bool gameOver();        // checks if the game is over
	void printState();      // prints each player's state at the end of a round
	void startGame();       // starts the game and play until there is a winner
	bool roundOver();       // checks if a round is over
	void dealCards();       // deals 13 cards to each player
	void newRound();        // starts a new round and play until the round ends
	void printTurn();       // prints information for each turn
	void play();            // facilitates the turns
	void printWinners();    // prints the winners of the game
};

#endif
