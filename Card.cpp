/**
 *	Card.cpp
 *	Function: Defines all functions declared in Card.h.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#include "stdafx.h"
#include "Card.h"

bool Card::operator== (const struct Card &card) const {
	if (suit == card.suit && rank == card.rank) {
		return true;
	}
	return false;
}

string Card::rankToString() {
	string name;
	switch (rank) {
	case two:
		name = "2";
		break;
	case three:
		name = "3";
		break;
	case four:
		name = "4";
		break;
	case five:
		name = "5";
		break;
	case six:
		name = "6";
		break;
	case seven:
		name = "7";
		break;
	case eight:
		name = "8";
		break;
	case nine:
		name = "9";
		break;
	case ten:
		name = "10";
		break;
	case jack:
		name = "J";
		break;
	case queen:
		name = "Q";
		break;
	case king:
		name = "K";
		break;
	case ace:
		name = "A";
		break;
	}

	return name;
}
string Card::suitToString() {
	string name;
	switch (suit) {
	case clubs:
		name = "C";
		break;
	case diamonds:
		name = "D";
		break;
	case hearts:
		name = "H";
		break;
	case spades:
		name = "S";
		break;
	}

	return name;
}
string Card::toString() {
	return rankToString() + suitToString();
}

bool Card::operator< (const Card &card) const {
	if (rank < card.rank) {
		return true;
	}

	else if (rank > card.rank) {
		return false;
	}

	else if (suit < card.suit) {
		return true;
	}

	return false;
}
ostream& operator<< (ostream &os, Card &card) {
	os << card.toString();
	return os;
}