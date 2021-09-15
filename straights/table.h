#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "subject.h"
#include "observer.h"

class Table : public Observer{
private:
	// holds the piles of cards on the table
	std::vector<std::vector<Card *>> piles;
public:
	Table();         // normal constructor
	~Table();        // normal destructor
	std::vector<std::vector<Card *>> getPiles() const;   // piles getter
	void printTable();                       // prints the table in text
	void placeCard(Card * card);	// places a card on the table
	void clearTable();              // clears the table
	void notify(Subject & whoNotified) override;
};

#endif
