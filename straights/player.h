#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "table.h"
#include "subject.h"
#include "observer.h"
#include "info.h"

enum TYPE {C = 1, H = 2, N = 3}; // indicates the type of a player with C = computer, H = human
                                 // N = unspecified

class Player : public Subject{
protected:
	// holds the hand of cards of a player
	std::vector<Card *> hand;
	// holds all cards discarded
	std::vector<Card *> discards;
	// holds the player's new and old scores
	int score;
	int oldscore;
	// the card currently played by the player
	Card * cur;
	// holds the set of legal plays
	std::vector<Card *> curLegal;
public:
	Player();            // Normal constructor
	~Player();           // Destructor
	virtual TYPE playertype() const = 0;      // returns the type of player
	std::vector<Card *> getHand() const;      // hand getter
	std::vector<Card *> getDiscards() const;  // discards getter
	int getScore() const;                      // score getter
	int getOldscore() const;                   // oldscore getter
	std::vector<Card *> getCurLegal() const;   // curLegal gettea
	Card * getCur() const;                     // cur getter
	void setHand(std::vector<Card *> h);      // hand setter
	void setCurLegal(Table t);   // curLegal setter
	bool isEmpty();      // check if the player has no more cards in hand
	void setScore();     // calculate the player's score gained
	void setOldscore(int s);  // oldscore setter
	void setDiscards(std::vector<Card *> d);   // discards setter
	void setCur(Card * c);                // cur setter
	void discard(Card * card);  // discard a card
	void removeCard(Card * card);          // removes a card from hand
	void reset();         // reset the state of the player
	void printCards(std::vector<Card *> c);  // prints c
	virtual Info getInfo() const override;
};

#endif
