#include "computerplayer.h"

// Normal constructor
ComputerPlayer::ComputerPlayer()
	: Player::Player()
{}

// return player type C
TYPE ComputerPlayer::playertype() const {
	return C;
}

// plays the first legal card in hand
void ComputerPlayer::playCard() {
	// if has legal play, plays the first legal play
	if (curLegal.size() != 0) {
		// set the first legal card as the current card being played
		cur = curLegal[0];
		notifyObservers();
		removeCard(cur);
	}
	else {
		// discard the first card in hand
		discard(getHand()[0]);
	}
}
