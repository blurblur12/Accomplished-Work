#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>

// define types for Suit and Rank of a card
enum Suit {CLUB, DIAMOND, HEART, SPADE};
enum Rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
	EIGHT, NINE, TEN, JACK, QUEEN, KING, X = 13};

class Card {
private:
	// holds information about the card
	Suit suit;
	Rank rank;
public:
	Card(Suit s, Rank r);  // normal constructor
	~Card();               // destructor
	Suit getSuit() const;  // suit getter
	Rank getRank() const;  // rank getter
	std::string textDisplay();             // prints the card in text form
};

bool operator==(Card & other1, Card & other);   // check if other is the same as this
bool operator==(Card * other1, const Card & other);   // overloaded, for a special application in game
Suit stringtoSuit(std::string st);      // converts string to a corresponding suit
Rank stringtoRank(std::string s);       // converts string to a corresponding rank
std::string suitoString(Suit s);        // converts suit to string
std::string ranktoString(Rank r);       // converts rank to string

#endif
