#include "humanplayer.h"

// Normal constructor
HumanPlayer::HumanPlayer()
	: Player::Player()
{}

// returns player type H
TYPE HumanPlayer::playertype() const {
	return H;
}

// plays the card that the user commands
void HumanPlayer::playCard(std::string input) {
	Card card{stringtoSuit(input.substr(1,1)), stringtoRank(input.substr(0,1))};
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (*(hand[i]) == card) {
			cur = hand[i];
			break;
		}
	}
	notifyObservers();
	removeCard(cur);
}

// discards the card that the user commands
void HumanPlayer::discardCard(std::string input) {
	Card card{stringtoSuit(input.substr(1,1)), stringtoRank(input.substr(0,1))};
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (*(hand[i]) == card) {
			discard(hand[i]);
			break;
		}
	}
}
