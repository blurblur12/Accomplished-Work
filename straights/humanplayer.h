#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "player.h"
#include "card.h"
#include <string>
#include <iostream>

class HumanPlayer : public Player {
public :
	HumanPlayer();        // Normal constructor
	//~HumanPlayer();       // Destructor
	virtual TYPE playertype() const override;    // returns player type H
	void playCard(std::string input);   // plays the card that the user commands
	void discardCard(std::string input);  // discards the card that the user commands
};

#endif
