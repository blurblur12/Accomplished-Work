#include "player.h"
#include "card.h"
#include "deck.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

// Normal constructor
Player::Player()
	: score{0},
	  oldscore{0},
	  cur{nullptr}
{}

// Destructor
Player::~Player() {
//	for (unsigned int i = 0; i < hand.size(); i++) {
//		delete hand[i];
//	}
	hand.clear();
	discards.clear();
	curLegal.clear();
}

// hand getter
std::vector<Card *> Player::getHand() const {
	return hand;
}

// discards getter
std::vector<Card *> Player::getDiscards() const {
	return discards;
}

// score getter
int Player::getScore() const {
	return score;
}

// oldscore getter
int Player::getOldscore() const {
	return oldscore;
}

// curLegal getter
std::vector<Card *> Player::getCurLegal() const {
	return curLegal;
}

// cur getter
Card * Player::getCur() const {
	return cur;
}

// hand setter
void Player::setHand(std::vector<Card *> h) {
	hand = h;
}

// curLegal setter
void Player::setCurLegal(Table t) {
	curLegal.clear();
	// if has 7S then it is the only legal play
	for (unsigned int i = 0; i < hand.size(); i++) {
	    Suit s = hand[i]->getSuit();
		Rank r = hand[i]->getRank();
		if(r == SEVEN && s == SPADE) {
		    curLegal.push_back(hand[i]);
		    return;
		}
	}
	for (unsigned int i = 0; i < hand.size(); i++) {
		Suit s = hand[i]->getSuit();
		Rank r = hand[i]->getRank();
		if (r == SEVEN) {
			curLegal.push_back(hand[i]);
		}
		else {
			if (t.getPiles()[s][0]->getRank() != 0) {
				unsigned int num = t.getPiles()[s].size();	
				if (r == Rank(t.getPiles()[s][0]->getRank() - 1) || 
						r ==  Rank(t.getPiles()[s][num - 1]->getRank() + 1)) {
					curLegal.push_back(hand[i]);
				}
			}
		}
	}
}

// check if the player has no more cards in hand
bool Player::isEmpty() {
	return hand.size() == 0;
}

// calcualte the player's score
void Player::setScore() {
	for (unsigned int i = 0; i < discards.size(); i++) {
		Rank r2 = discards[i]->getRank();
		score = score + (Rank)r2 + 1;
	}
}

// oldscore setter
void Player::setOldscore(int s) {
	oldscore = s;
}

// discards setter
void Player::setDiscards(std::vector<Card *> d) {
    discards = d;
}

// cur setter
void Player::setCur(Card * c) {
    cur = c;
}

// discard a card
void Player::discard(Card * card) {
	std::cout << ranktoString(card->getRank()) << suitoString(card->getSuit()) 
		<< "." << std::endl;
	discards.push_back(card);
	removeCard(card);
}

// removes a card from hand
void Player::removeCard(Card * card) {
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (*(hand[i]) == *card) {
			hand.erase(hand.begin() + i);
		}
	}
}

// resets the state of the player
void Player::reset() {
	score = 0;
//	for (unsigned int i = 0; i < hand.size(); i++) {
//		delete hand[i];
//	}
	hand.clear();
	discards.clear();
	curLegal.clear();
}

// prints cards c
void Player::printCards(std::vector<Card *> c) {
	for (unsigned int i = 0; i < c.size(); i++) {
		std::cout << " " << c[i]->textDisplay();
	}
	std::cout << std::endl;
}

Info Player::getInfo() const {
    Info i = {};
    i.cards = std::move(cur);
    return i;
}
