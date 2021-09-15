#include "deck.h"
#include "card.h"
#include <algorithm>
#include <random>

// Normal constructor, set the deck to standard order
Deck::Deck(int seed)
	: seed{seed}
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			fulldeck.push_back(new Card((Suit)i, (Rank)j));
		}
	}
}

// Destructor
Deck::~Deck() {
	for (unsigned int i = 0; i < fulldeck.size(); i++) {
		delete fulldeck[i];
	}
	fulldeck.clear();
}

// fulldeck getter
std::vector<Card *>  Deck::getFulldeck(int f, int l) const{
	std::vector<Card *> h;
    std::copy(fulldeck.begin()+f, fulldeck.begin()+l, back_inserter(h));
    return h;
}

// seed setter
void Deck::setSeed(int seed) {
	this->seed = seed;
}

// shuffles the deck
void Deck::shuffle() {
	std::default_random_engine rng{seed};
	std::shuffle(fulldeck.begin(), fulldeck.end(), rng);
}

// prints the whole deck
void Deck::printDeck() {
	for (int i = 0; i < 52; i++) {
		if(i % 13 == 0 && i != 0) {
			std::cout << std::endl;
		}
		std::cout << fulldeck[i]->textDisplay();
		if (!(i == 12 || i == 25 || i == 38 || i == 51)) {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}
