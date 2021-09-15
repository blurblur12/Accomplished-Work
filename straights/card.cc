#include "card.h"

// Normal constructor
Card::Card(Suit s, Rank r)
        : suit{s},
          rank{r}
{}

// Destructor
Card::~Card() {}

// suit getter
Suit Card::getSuit() const{
        return suit;
}

// rank getter
Rank Card::getRank() const{
        return rank;
}

// checks if other is the same card as this
bool operator==(Card & other1, Card & other) {
        return other1.getSuit() == other.getSuit() && other1.getRank() == other.getRank();
}

// overloaded ==
bool operator==(Card * other1, const Card & other) {
    return other1->getSuit() == other.getSuit() && other1->getRank() == other.getRank();
}

// prints the card in text form
std::string Card::textDisplay() {
	return ranktoString(rank) + suitoString(suit);
}

// converts string to the corresponding suit
Suit stringtoSuit(std::string st) {
        Suit s = CLUB;
        if (st == "C") {
                s = CLUB;
        }
        else if (st == "D") {
                s = DIAMOND;
        }
        else if (st == "H") {
                s = HEART;
        }
        else if (st == "S") {
                s = SPADE;
        }
        // else {}
        return s;
}

// converts string to the corresponding rank
Rank stringtoRank(std::string s) {
        Rank r = X;
        if (s == "A") {
                r = ACE;
        }
        else if (s == "2") {
                r = TWO;
        }
        else if (s == "3") {
                r = THREE;
        }
        else if (s == "4") {
                r = FOUR;
        }
        else if (s == "5") {
                r = FIVE;
        }
        else if (s == "6") {
                r = SIX;
        }
        else if (s == "7") {
                r = SEVEN;
        }
        else if (s == "8") {
                r = EIGHT;
        }
        else if (s == "9") {
                r = NINE;
        }
        else if (s == "T") {
                r = TEN;
        }
        else if (s == "J") {
                r = JACK;
        }
        else if (s == "Q") {
                r = QUEEN;
        }
        else if (s == "K") {
                r = KING;
        }
	//else {}
        return r;
}

// converts suit to a corresponding string representation
std::string suitoString(Suit s) {
        std::string st = "";
        if (s == CLUB) {
                st = "C";
        }
        else if (s == DIAMOND) {
                st = "D";
        }
        else if (s == HEART) {
                st = "H";
        }
        else if (s == SPADE) {
                st = "S";
        }
        //else {}
        return st;
}

// converts rank to a corresponding string representation
std::string ranktoString(Rank r) {
    	std::string s = "";
    	if (r == ACE) {
                s = "A";
        }
        else if (r == TWO) {
                s = "2";
        }
        else if (r == THREE) {
                s = "3";
        }
        else if (r == FOUR) {
                s = "4";
        }
        else if (r == FIVE) {
                s = "5";
        }
        else if (r == SIX) {
                s = "6";
        }
        else if (r == SEVEN) {
                s = "7";
        }
        else if (r == EIGHT) {
                s = "8";
        }
        else if (r == NINE) {
                s = "9";
        }
        else if (r == TEN) {
                s = "T";
        }
        else if (r == JACK) {
                s = "J";
        }
        else if (r == QUEEN) {
                s = "Q";
        }
    	else if (r == KING) {
                s = "K";
        }
	//else {}
        return s;
}
