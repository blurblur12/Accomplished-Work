#include "table.h"
#include <utility>

// Normal constructor
Table::Table() {
	// creates 4 piles one for each suit, each pile
	// contains a card of unspecified rank X
	for (int i = 0; i < 4; i++) {
		std::vector<Card *> p;
		p.emplace_back(new Card(Suit(i), Rank(13)));
		piles.emplace_back(p);
	}
}

// Destructor
Table::~Table() {
	for (unsigned int i = 0; i < piles.size(); i++) {
//		for (unsigned int j = 0; j < piles[i].size(); j++) {
//			delete piles[i][j];
//		}
		piles[i].clear();
	}
	piles.clear();
}

// piles getter
std::vector<std::vector<Card *>> Table::getPiles() const {
	return piles;
}

// prints the table in text
void Table::printTable() {
	std::cout << "Cards on the table:" << std::endl;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			std::cout << "Clubs: ";
		}
		else if (i == 1) {
			std::cout << "Diamonds: ";
		}
		else if (i == 2) {
			std::cout << "Hearts: ";
		}
		else if (i == 3) {
			std::cout << "Spades: ";
		}
		else {}

		for (unsigned int j = 0; j < piles[i].size(); j++) {
			std::cout << ranktoString(piles[i][j]->getRank());
			if (j != piles[i].size() - 1) {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

// places a card on the table
void Table::placeCard(Card * card) {
	int s = card->getSuit();
	int r = card->getRank();
	Suit s2 = card->getSuit();
	Rank r2 = card->getRank();
	std::cout << ranktoString(r2) << suitoString(s2) << "." << std::endl;
	// if the pile is not initialized, then initialize it
	if (piles[s][0]->getRank() == X) {
		delete piles[s][0];
		piles[s].clear();
		piles[s].push_back(card);
	}
	else {
		// insert the card into the pile in correct order
		if (r < piles[s][0]->getRank()) {
			piles[s].insert(piles[s].begin(), card);
		}
		else if (r > piles[s][piles[s].size() - 1]->getRank()) {
			piles[s].push_back(card);
		}
		else {}
	}
}

// clears the table
void Table::clearTable() {
	for (unsigned int i = 0; i < piles.size(); i++) {
//		for (unsigned int j = 0; j < piles[i].size(); j++) {
//			delete piles[i][j];
//		}
		piles[i].clear();
	}
	piles.clear();
	// re-initialize the piles such that each suit contains a card of rank unknown X
	for (int i = 0; i < 4; i++) {
		std::vector<Card *> p;
		p.emplace_back(new Card(Suit(i), Rank(13)));
		piles.emplace_back(p);
	}
}

// add the card being played
void Table::notify(Subject & whoNotified) {
	Info i = whoNotified.getInfo();
	placeCard(i.cards);
}
