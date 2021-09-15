#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <vector>

class Deck {
private:
	// holds a full deck of 52 cards
	std::vector<Card *> fulldeck;
	// the seed used to shuffle the cards
	int seed;
public:
	Deck(int seed); // normal constructor
	~Deck();     // Destructor;
	std::vector<Card *> getFulldeck(int f, int l) const;  // fulldeck getter
	void setSeed(int seed);      // seed setter
	void shuffle();              // shuffles the deck
	void printDeck();           // prints the whole deck
};

#endif
