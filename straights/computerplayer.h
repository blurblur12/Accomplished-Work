#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "player.h"
#include "card.h"


class ComputerPlayer : public Player {
public :
	ComputerPlayer();      // Normal constructor
	//~ComputerPlayer();     // Destructor
	virtual TYPE playertype() const override;   // returns player type C
	void playCard();    // plays the first legal play in hand
};

#endif
